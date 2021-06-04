Ceph up_thru
============
思考下类极端场景：

Ceph集群中某个PG有两个osd（`osd.1`、`osd.2`），在某个时刻`osd.1`和`osd.2`都是最新副本，此时`osd.1`如果挂掉，随后`osd.2`也挂掉，接着`osd.1`又重新上线，那么此时`osd.1`是否能提供读写服务呢？

|        | osd.1  | osd.2  |
| :----: | :----: | :----: |
| epoch1 | **up** | **up** |
| epoch2 |~~down~~| **up** |
| epoch3 |~~down~~|~~down~~|
| epoch3 | **up** |~~down~~|

在`osd.1`挂掉期间：
 - 如果`osd.2`有数据更新，那么`osd.1`重新上线后显然不能提供读写服务的，因为其上的副本不是最新的。
 - 如果`osd.2`没有数据更新，那么`osd.1`重新上线后是可以提供读写服务的。

# up_thru到底是啥？
首先，up_thru相关数据结构如下：

./src/osd/OSDMap.h
```C++
struct osd_info_t {
    epoch_t last_clean_begin; // last interval that ended with a clean osd shutdown
    epoch_t last_clean_end;
    epoch_t up_from;          // epoch osd marked up
    epoch_t up_thru;          // lower bound on actual osd death (if > up_from)
    epoch_t down_at;          // upper bound on actual osd death (if > up_from)
    epoch_t lost_at;          // last epoch we decided data was "lost"
    ...
};

class OSDMap{
    ...

private:
    vector<osd_info_t> osd_info;
    ceph::shared_ptr< map<pg_t,vector<int32_t> > > pg_temp;  // temp pg mapping (e.g. while we rebuild)
    ceph::shared_ptr< map<pg_t,int32_t > > primary_temp;  // temp primary mapping (e.g. while we rebuild)

    ...
};
```

通过数据结构可以知道，up_thru是保存在`OSD Map`里的一个（epoch_t类型）版本号。

在OSD处于alive且healthy期间通常会跟踪两个interval：
 - 最近的是[up_from, up_thru)，这里up_thru（如果 > up_from）是已知OSD处于`_started_`状态时的最后一个epoch，即OSD实际down的下限，而down_at（如果 > up_from）则是OSD实际down的上限。
 - 另一个是last_clean interval[begin，end)，这时last interval是已知OSD处于`_finished_`状态时或cleanly shut down的最后epoch。在可能的情况下，需将其向前推进到OSD最终epoch。

lost_at用于允许build_prior继续进行而无需等待OSD恢复。在某些情况下，进程可能会被阻塞，因为OSD处于down状态时可能包含更新（例如：PG在某个时间间隔内可能已经在读写）。如果OSD无法回到online状态，可以在知道丢失某些已确认的写入的情况下迫使事情继续进行。即使OSD稍后恢复正常，但是那些写操作仍然会丢失（分歧对象将被丢弃）。

通过`ceph osd dump`查看`OSD Map`信息如下：
```shell
osd.1 up   in  weight 1 up_from 12771 up_thru 13289 down_at 12764 last_clean_interval [12742,12763) ...
osd.2 up   in  weight 1 up_from 12770 up_thru 13289 down_at 12764 last_clean_interval [12741,12763) ...
```

# up_thru的更新
继续上文开头的那个例子，当`osd.1`挂掉之后，整个集群反应如下：
 - `osd.1`挂掉之后，`osd.1`主动上报或者其他osd向`monitor`上报，此时`monitor`感知到`osd.1`挂掉。
 - `monitor`将`OSD Map`中`osd.1`标记为`down`，并将新的`OSD Map`发送给`osd.2`。
 - `osd.2`（第一次）收到新的`OSD Map`后，相关`PG`开始`peering`，若`PG`判断需要向`monitor`申请更新`up_thru`信息，那么`PG`状态变为`WaitUpThru`。
 - `monitor`收到更新`up_thru`信息后，更新`OSD Map`里面`osd.2`的`up_thru`，并将新的`OSD Map`发送给`osd.2`。
 - `osd.2`（第二次）收到新的`OSD Map`后，相关`PG`状态由`WaitUpThru`变为`Active`，可以开始提供IO服务。


## OSD更新up_thru处理流程
`PG`判断是否需要向`monitor`申请更新`up_thru`消息，若需要则向`monitor`发送该消息，如下：
```C++
PastIntervals::PriorSet PeeringState::build_prior()
{
  if (1) {
    // sanity check
    for (auto it = peer_info.begin(); it != peer_info.end(); ++it) {
      ceph_assert(info.history.last_epoch_started >=
                  it->second.history.last_epoch_started);
    }
  }

  const OSDMap &osdmap = *get_osdmap();
  PastIntervals::PriorSet prior = past_intervals.get_prior_set(
    pool.info.is_erasure(),
    info.history.last_epoch_started,
    &missing_loc.get_recoverable_predicate(),
    [&](epoch_t start, int osd, epoch_t *lost_at) {
      const osd_info_t *pinfo = 0;
      if (osdmap.exists(osd)) {
        pinfo = &osdmap.get_info(osd);
        if (lost_at)
          *lost_at = pinfo->lost_at;
      }

      if (osdmap.is_up(osd)) {
        return PastIntervals::UP;
      } else if (!pinfo) {
        return PastIntervals::DNE;
      } else if (pinfo->lost_at > start) {
        return PastIntervals::LOST;
      } else {
        return PastIntervals::DOWN;
      }
    },
    up,
    acting,
    dpp);

  if (prior.pg_down) {
    state_set(PG_STATE_DOWN);
  }

  /*
  osd.1挂掉后PG的Up Set会发生变化（也即产生新的past interval），
  此时会更新info.history.same_interval_since为新的OSD Map版本号，
  因为same_interval_since表示最近一个past interval的第一个OSD Map版本号。

  osd.1挂掉后肯定满足up_thru < info.history.same_interval_since。
  */

  if (get_osdmap()->get_up_thru(pg_whoami.osd) <
      info.history.same_interval_since) {
    psdout(10) << "up_thru " << get_osdmap()->get_up_thru(pg_whoami.osd)
               << " < same_since " << info.history.same_interval_since
               << ", must notify monitor" << dendl;
    need_up_thru = true;
  } else {
    psdout(10) << "up_thru " << get_osdmap()->get_up_thru(pg_whoami.osd)
               << " >= same_since " << info.history.same_interval_since
               << ", all is well" << dendl;
    need_up_thru = false;
  }
  pl->set_probe_targets(prior.probe);
  return prior;
}

/* 判断是否需要更新up_thru。 */
void OSD::dequeue_peering_evt(
  OSDShard *sdata,
  PG *pg,
  PGPeeringEventRef evt,
  ThreadPool::TPHandle& handle)
{
  PeeringCtx rctx = create_context();
  auto curmap = sdata->get_osdmap();
  bool need_up_thru = false;
  epoch_t same_interval_since = 0;
  if (!pg) {
    if (const MQuery *q = dynamic_cast<const MQuery*>(evt->evt.get())) {
      handle_pg_query_nopg(*q);
    } else {
      derr << __func__ << " unrecognized pg-less event " << evt->get_desc() << dendl;
      ceph_abort();
    }
  } else if (advance_pg(curmap->get_epoch(), pg, handle, rctx)) {
    pg->do_peering_event(evt, rctx);
    if (pg->is_deleted()) {
      pg->unlock();
      return;
    }
    dispatch_context(rctx, pg, curmap, &handle);
    need_up_thru = pg->get_need_up_thru();
    same_interval_since = pg->get_same_interval_since();
    pg->unlock();
  }

  if (need_up_thru) {
    queue_want_up_thru(same_interval_since);
  }

  service.send_pg_temp();
}

/* 若待发送up_thru大于已发送值，则向monitor发送消息。 */
void OSD::queue_want_up_thru(epoch_t want)
{
  std::shared_lock map_locker{map_lock};
  epoch_t cur = get_osdmap()->get_up_thru(whoami);
  std::lock_guard report_locker(mon_report_lock);
  if (want > up_thru_wanted) {
    dout(10) << "queue_want_up_thru now " << want << " (was " << up_thru_wanted << ")"
             << ", currently " << cur
             << dendl;
    up_thru_wanted = want;
    send_alive();
  } else {
    dout(10) << "queue_want_up_thru want " << want << " <= queued " << up_thru_wanted
             << ", currently " << cur
             << dendl;
  }
}

/* 向monitor发送消息。 */
void OSD::send_alive()
{
  ceph_assert(ceph_mutex_is_locked(mon_report_lock));
  const auto osdmap = get_osdmap();
  if (!osdmap->exists(whoami))
    return;
  epoch_t up_thru = osdmap->get_up_thru(whoami);
  dout(10) << "send_alive up_thru currently " << up_thru << " want " << up_thru_wanted << dendl;
  if (up_thru_wanted > up_thru) {
    dout(10) << "send_alive want " << up_thru_wanted << dendl;
    monc->send_mon_message(new MOSDAlive(osdmap->get_epoch(), up_thru_wanted));
  }
}
```

## monitor更新up_thru处理流程
monitor接收消息并处理，如下：
```C++
// -------------
// alive

bool OSDMonitor::preprocess_alive(MonOpRequestRef op)
{
  op->mark_osdmon_event(__func__);
  auto m = op->get_req<MOSDAlive>();
  int from = m->get_orig_source().num();

  // check permissions, ignore if failed
  MonSession *session = op->get_session();
  if (!session)
    goto ignore;
  if (!session->is_capable("osd", MON_CAP_X)) {
    dout(0) << "attempt to send MOSDAlive from entity with insufficient privileges:"
            << session->caps << dendl;
    goto ignore;
  }

  if (!osdmap.is_up(from) ||
      !osdmap.get_addrs(from).legacy_equals(m->get_orig_source_addrs())) {
    dout(7) << "preprocess_alive ignoring alive message from down "
            << m->get_orig_source() << " " << m->get_orig_source_addrs()
            << dendl;
    goto ignore;
  }

  /* 如果最新的OSD Map中该OSD的up_thru大于等于OSD想要申请的。
  那么monitor不需要决议，只需要把OSD缺失的所有OSD Map发送给它。 */
  if (osdmap.get_up_thru(from) >= m->want) {
    // yup.
    dout(7) << "preprocess_alive want up_thru " << m->want << " dup from " << m->get_orig_source_inst() << dendl;
    _reply_map(op, m->version);
    return true;
  }

  dout(10) << "preprocess_alive want up_thru " << m->want
           << " from " << m->get_orig_source_inst() << dendl;
  return false;

 ignore:
  return true;
}

/* 若monitor需要决议，则进行决议。 */
bool OSDMonitor::prepare_alive(MonOpRequestRef op)
{
  op->mark_osdmon_event(__func__);
  auto m = op->get_req<MOSDAlive>();
  int from = m->get_orig_source().num();

  if (0) {  // we probably don't care much about these
    mon.clog->debug() << m->get_orig_source_inst() << " alive";
  }

  dout(7) << "prepare_alive want up_thru " << m->want << " have " << m->version
          << " from " << m->get_orig_source_inst() << dendl;

  update_up_thru(from, m->version); // set to the latest map the OSD has
  /* 决议完成后回调C_ReplyMap，将新的OSD Map发给OSD。 */
  wait_for_finished_proposal(op, new C_ReplyMap(this, op, m->version));
  return true;
}

void OSDMonitor::update_up_thru(int from, epoch_t up_thru)
{
  epoch_t old_up_thru = osdmap.get_up_thru(from);
  auto ut = pending_inc.new_up_thru.find(from);
  if (ut != pending_inc.new_up_thru.end()) {
    old_up_thru = ut->second;
  }
  if (up_thru > old_up_thru) {
    // set up_thru too, so the osd doesn't have to ask again
    pending_inc.new_up_thru[from] = up_thru;
  }
}

/* monitor决议完后，最终会调用下面的函数更新OSDMap。 */
int OSDMap::apply_incremental(const Incremental &inc)
{
  ···

  // info
  /* 更新osdmap中该osd对应的up_thru字段。 */
  for (const auto &thru : inc.new_up_thru)
    osd_info[thru.first].up_thru = thru.second;
  
  for (const auto &interval : inc.new_last_clean_interval) {
    osd_info[interval.first].last_clean_begin = interval.second.first;
    osd_info[interval.first].last_clean_end = interval.second.second;
  }
  
  for (const auto &lost : inc.new_lost)
    osd_info[lost.first].lost_at = lost.second;

  // xinfo
  for (const auto &xinfo : inc.new_xinfo)
    osd_xinfo[xinfo.first] = xinfo.second;

  // uuid
  for (const auto &uuid : inc.new_uuid)
    (*osd_uuid)[uuid.first] = uuid.second;

  // pg rebuild
  for (const auto &pg : inc.new_pg_temp) {
    if (pg.second.empty())
      pg_temp->erase(pg.first);
    else
      pg_temp->set(pg.first, pg.second);
  }
  if (!inc.new_pg_temp.empty()) {
    // make sure pg_temp is efficiently stored
    pg_temp->rebuild();
  }

  for (const auto &pg : inc.new_primary_temp) {
    if (pg.second == -1)
      primary_temp->erase(pg.first);
    else
      (*primary_temp)[pg.first] = pg.second;
  }

  for (auto& p : inc.new_pg_upmap) {
    pg_upmap[p.first] = p.second;
  }
  for (auto& pg : inc.old_pg_upmap) {
    pg_upmap.erase(pg);
  }
  for (auto& p : inc.new_pg_upmap_items) {
    pg_upmap_items[p.first] = p.second;
  }
  for (auto& pg : inc.old_pg_upmap_items) {
    pg_upmap_items.erase(pg);
  }

  ···
}
```

## PG状态转换处理流程
`osd.2`（第一次）收到新的`OSD Map`后，相关`PG`在`peering`中如果需要`monitor`更新`up_thru`，那么`PG`状态会变为`WaitUpThru`，如下：
```C++
/*------GetMissing--------*/
PeeringState::GetMissing::GetMissing(my_context ctx)
  : my_base(ctx),
    NamedState(context< PeeringMachine >().state_history, "Started/Primary/Peering/GetMissing")
{
  ...

  if (peer_missing_requested.empty()) {
    if (ps->need_up_thru) {
      psdout(10) << " still need up_thru update before going active"
                 << dendl;
      post_event(NeedUpThru());
      return;
    }

    // all good!
    post_event(Activate(ps->get_osdmap_epoch()));
  } else {
    pl->publish_stats_to_osd();
  }
}
```

正处于`WaitUpThru`状态的`PG`收到新的`OSD Map`后，会将need_up_thru置为false，如下：
```C++
#define DECLARE_LOCALS                                  \
  PeeringState *ps = context< PeeringMachine >().state; \
  std::ignore = ps;                                     \
  PeeringListener *pl = context< PeeringMachine >().pl; \
  std::ignore = pl

boost::statechart::result PeeringState::Peering::react(const AdvMap& advmap)
{
  DECLARE_LOCALS;
  psdout(10) << "Peering advmap" << dendl;
  /* 收到AdvMap事件后，首先检查OSD Map是否影响prio，若影响则转移至reset */
  if (prior_set.affected_by_map(*(advmap.osdmap), ps->dpp)) {
    psdout(1) << "Peering, affected_by_map, going to Reset" << dendl;
    post_event(advmap);
    return transit< Reset >();
  }

  /* 将need_up_thru置为false */
  ps->adjust_need_up_thru(advmap.osdmap);
  ps->check_prior_readable_down_osds(advmap.osdmap);

  return forward_event();
}

bool PeeringState::adjust_need_up_thru(const OSDMapRef osdmap)
{
  epoch_t up_thru = osdmap->get_up_thru(pg_whoami.osd);
  if (need_up_thru &&
      up_thru >= info.history.same_interval_since) {
    psdout(10) << "adjust_need_up_thru now "
               << up_thru << ", need_up_thru now false" << dendl;
    need_up_thru = false;
    return true;
  }
  return false;
}
```

当正处于`WaitUpThru`状态的`PG`收到`ActMap`事件后，由于need_up_thru标记已经被置为false，所以进入active状态。进入active状态后，就可以开始提供IO服务了，如下：
```C++
boost::statechart::result PeeringState::WaitUpThru::react(const ActMap& am)
{
  DECLARE_LOCALS;
  if (!ps->need_up_thru) {
    post_event(Activate(ps->get_osdmap_epoch()));
  }
  return forward_event();
}
```

# up_thru的应用
上文描述了当`osd.1`挂掉后，整个集群的响应，那么`osd.1`重新上线后能否提供服务？

假设初始时刻，也即`osd.1`和`osd.2`都健康，`PG`都是active+clean状态时`OSD Map`的`epoch`为100，此时`osd.1`和`osd.2`的`up_thru`必然都小于100。
 1. `osd.1`不能提供IO服务的情况：
    - `osd.1`挂掉，`OSD Map`的`epoch`变为101；
    - `osd.2`向`monitor`申请更新`up_thru`成功，`OSD Map`的`epoch`变为102，`OSD Map`中`osd.2`的`up_thru`变为101，由于`osd.2`收到新的`OSD Map`后，`PG`的状态就可以变为`active`（即可以提供IO服务），所以如果`up_thru`更新成功意味着`osd.2`可能有新的写入（当然也可能没有新的写入，但是无法排除存在新的写入的可能）；
    - `osd.2`挂掉，`OSD Map`的`epoch`变为103；
    - `osd.1`重新上线，`OSD Map`的`epoch`变为104；
 2. `osd.1`能提供IO服务的情况：
    - `osd.1`挂掉，`OSD Map`的`epoch`变为101；
    - `osd.2`没有向`monitor`申请更新`up_thru`或申请更新失败（比如申请更新之前`osd.2`就挂了）
    - `osd.2`挂掉，`OSD Map`的`epoch`变为102；
    - `osd.1`重新上线，`OSD Map`的`epoch`变为103；

# 总结
在文章开头我们提出了一个疑问，即Ceph集群中有1个osd down了，那么`OSD Map`会发生什么变化？`OSD Map`会更新几次？

答案是：不一定，主要是分以下两种情况：
 - 该挂掉的osd与其他osd无共同承载的PG
    此时`OSD Map`只会更新1次，变化便是`OSD Map`中osd的状态从up更新为down。因为都不存在相关PG，也就不存在peering，也就没有up_thru的更新了，所以`OSD Map`变化1次。
 - 该挂掉的osd与其他osd有共同承载的PG
    此时`OSD Map`会至少更新两次，其中第1次是更新`OSD Map`中osd的状态，第2次便是更新相关osd的up_thru。

# 补充
在OSDMonitor端，还有一种情况会触发up_thru的变更：

```C++
bool OSDMonitor::prepare_pgtemp(MonOpRequestRef op)
{
    op->mark_osdmon_event(__func__);
    MOSDPGTemp *m = static_cast<MOSDPGTemp*>(op->get_req());
    int from = m->get_orig_source().num();
    dout(7) << "prepare_pgtemp e" << m->map_epoch << " from " << m->get_orig_source_inst() << dendl;

    for (map<pg_t,vector<int32_t> >::iterator p = m->pg_temp.begin(); p != m->pg_temp.end(); ++p) {
        uint64_t pool = p->first.pool();
        if (pending_inc.old_pools.count(pool)) {
            dout(10) << __func__ << " ignore " << p->first << " -> " << p->second<< ": pool pending removal" << dendl;
            continue;
        }
        if (!osdmap.have_pg_pool(pool)) {
            dout(10) << __func__ << " ignore " << p->first << " -> " << p->second<< ": pool has been removed" << dendl;
            continue;
        }

        pending_inc.new_pg_temp[p->first] = p->second;

        // unconditionally clear pg_primary (until this message can encode
        // a change for that, too.. at which point we need to also fix
        // preprocess_pg_temp)
        if (osdmap.primary_temp->count(p->first) ||
          pending_inc.new_primary_temp.count(p->first))
            pending_inc.new_primary_temp[p->first] = -1;
    }

    // set up_thru too, so the osd doesn't have to ask again
    update_up_thru(from, m->map_epoch);

    wait_for_finished_proposal(op, new C_ReplyMap(this, op, m->map_epoch));
    return true;
}
```

由如上代码，对应的OSD请求pg_temp时，触发更新该OSD的up_thru。


# 参考
 * [聊聊Ceph之up_thru来龙去脉分析](https://zhuanlan.zhihu.com/p/165471449)
 * [ceph之up_thru分析](https://ivanzz1001.github.io/records/post/ceph/2017/07/21/ceph-up_thru)
 * [ceph peering机制-过程分析](https://blog.51cto.com/wendashuai/2513897)
