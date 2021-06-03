Ceph Peering
============

# 概念
 - [Acting Set](./ceph-concept/ceph-concept-acting_set.md)
 - [Up Set](./ceph-concept/ceph-concept-up_set.md)
 - [current interval](./ceph-concept/ceph-concept-current_interval.md)
 - [past interval](./ceph-concept/ceph-concept-past_interval.md)
 - [stray](./ceph-concept/ceph-concept-stray.md)
 - [Authoritative History](./ceph-concept/ceph-concept-authoritative_history.md)
 - [last epoch start](./ceph-concept/ceph-concept-last_epoch_start.md)
 - [last epoch clean](./ceph-concept/ceph-concept-last_epoch_clean.md)
 - [up_thru](./ceph-concept/ceph-concept-up_thru.md)

## Peering Interval
请参见：
 - `PG::start_peering_interval`
 - `PG::acting_up_affected`
 - `PG::PeeringState::Reset`

`Peering Interval`是一组最大连续映射（Acting Set和Up Set未改变）epochs的集合。`PG::PeeringMachine`通过`PeeringState::Reset`从一个间隔转换到另一个间隔，而`PG::PeeringState::AdvMap`和`PG::acting_up_affected`会导致PG转换为Reset。

# PEERING DETAILS AND GOTCHAS
PG:：flushed默认为false，并在PG:：start\u peering\u interval中设置为false。在转换到PG：：PeeringState：：Started时，我们通过PG op sequencer发送一个事务，该事务完成后，发送一个FlushedEvt，将flushed设置为true。在这种情况发生之前，主服务器不能处于活动状态（请参阅PG:：PeeringState:：WaitFlushedPeering）。复制副本可以处于活动状态，但不能服务于操作（写入或读取）。这是必要的，因为在前一个时间间隔的不稳定事务清除之前，我们无法读取ondisk状态。

`PG::flushed`默认为false，并在`PG::start_peering_interval`中设置为false。在转换到`PG::PeeringState::Started`后，通过PG op sequencer发送一个事务，该事务完成后发送一个`FlushedEvt`，将`PG::flushed`设置为true。在`PG::flushed`设置为true之前primary无法进入active状态（参见`PG::PeeringState::WaitFlushedPeering`），replicas则可以进入active状态但不能提供读写操作。这是必要的，因为在前一个时间间隔的不稳定事务被清除之前，是不能够读取磁盘状态的。

# 数据结构
## pg_info_t
```C++
/**
 * pg_info_t - summary of PG statistics.
 *
 * some notes: 
 *  - last_complete implies we have all objects that existed as of that
 *    stamp, OR a newer object, OR have already applied a later delete.
 *  - if last_complete >= log.tail, then we know pg contents thru log.head.
 *    otherwise, we have no idea what the pg is supposed to contain.
 */
struct pg_info_t {
  spg_t pgid;
  eversion_t last_update;      ///< last object version applied to store.
  eversion_t last_complete;    ///< last version pg was complete through.
  epoch_t last_epoch_started;  ///< last epoch at which this pg started on this osd
  epoch_t last_interval_started; ///< first epoch of last_epoch_started interval
  
  version_t last_user_version; ///< last user object version applied to store

  eversion_t log_tail;         ///< oldest log entry.

  hobject_t last_backfill;     ///< objects >= this and < last_complete may be missing

  interval_set<snapid_t> purged_snaps;

  pg_stat_t stats;

  pg_history_t history;
  pg_hit_set_history_t hit_set;

  ···
};
```

## pg_history_t
```C++
/**
 * pg_history_t - information about recent pg peering/mapping history
 *
 * This is aggressively shared between OSDs to bound the amount of past
 * history they need to worry about.
 */
struct pg_history_t {
  epoch_t epoch_created = 0;        // epoch in which *pg* was created (pool or pg)
  epoch_t epoch_pool_created = 0;   // epoch in which *pool* was created
                                    // (note: may be pg creation epoch for
                                    // pre-luminous clusters)
  epoch_t last_epoch_started = 0;;  // lower bound on last epoch started (anywhere, not necessarily locally)
                                    // https://docs.ceph.com/docs/master/dev/osd_internals/last_epoch_started/
  epoch_t last_interval_started = 0;; // first epoch of last_epoch_started interval
  epoch_t last_epoch_clean = 0;;    // lower bound on last epoch the PG was completely clean.
  epoch_t last_interval_clean = 0;; // first epoch of last_epoch_clean interval
  epoch_t last_epoch_split = 0;;    // as parent or child
  epoch_t last_epoch_marked_full = 0;;  // pool or cluster

  /**
   * In the event of a map discontinuity, same_*_since may reflect the first
   * map the osd has seen in the new map sequence rather than the actual start
   * of the interval.  This is ok since a discontinuity at epoch e means there
   * must have been a clean interval between e and now and that we cannot be
   * in the active set during the interval containing e.
   */
  epoch_t same_up_since = 0;;       // same acting set since
  epoch_t same_interval_since = 0;;   // same acting AND up set since
  epoch_t same_primary_since = 0;;  // same primary at least back through this epoch.

  eversion_t last_scrub;
  eversion_t last_deep_scrub;
  utime_t last_scrub_stamp;
  utime_t last_deep_scrub_stamp;
  utime_t last_clean_scrub_stamp;

  ···
};
```

# Peering过程
黄金法则是，对任何`PG`的写入操作在被该`PG`的`Acting Set`的所有`OSD`持久化之前都不会向通知客户端已确认。这意味着如果可以与自最后成功`peering`以来的每个`Acting Set`中的至少一个`OSD`通信，则其会拥有自最后成功`peering`以来的每个（已确认）操作记录，这也就意味着当前`primary`应该有可能重建和散播新的权威日志（`authoritative history`）。

了解`OSD Map`（所有已知`OSD`列表及其状态的，以及有关`PG`的一些信息）在`peering`过程中的作用也很重要：
 - 当`OSD`发生up或down（或add或remove）时，这有可能影响许多`PG`的`Acting Set`。
 - 在`primary`成功完成`peering`过程之前，`OSD Map`必须反映`OSD`是存活的，并且是`current interval`中的第一个`epoch`。
 - 只有在`peering`成功后才能进行更改。

因此，新的`primary`可以使用最新的`OSD Map`以及`OSD Map`的近期历史来生成一组`past interval`集合，以确定在`peering`成功之前必须询问哪些`OSD`。`past interval`集合以`pg_info_t:last_epoch_started`为界，即知道`peering`完成的最近`past interval`。`OSD`发现`PG`存在是通过交换`PG Info`消息，因此`OSD`总是有`pg_history_t:last_epoch_started`下限值。

当前`PG`的`primary`处理流程：
 1. 获取最近的`OSD Map`（以识别所有`Acting Set`的成员，并确认自身仍然是`primary`）。
 2. 生成自`pg_info_t:last_epoch_started`以来的`past intervals`列表。考虑那些间隔的最后一个`epoch`的`OSD Map`中`up_thru`大于间隔的第一个`epoch`的子集；也就是说，在`Acting Set`变化为另一组`OSD`之前，`peering`可能已完成的子集。
 `peering`成功要求能够从每个`past interval`的`Acting Set`中联系至少一个`OSD`。
 3. 向该列表中的每个节点询问其`PG Info`，其中包括对`PG`进行的最近写入，以及`pg_info_t:last_epoch_started`值。如果得知`pg_info_t:last_epoch_started`比自身拥有的更新，可以裁剪更旧的`past interval`并减少`peering`需要联系的`OSD`。
 4. 如果在其他`OSD`的`PG Log`中有`primary`没有的操作，就指示该`OSD`向`primary`发送缺失的`PG Log`条目，以使`primary`的`PG Log`保持最新（包括最新的写入）。
 5. 对于当前`Acting Set`的每个`OSD`：
    - 要求其提供自`pg_info_t:last_epoch_started`以来所有的`PG Log`条目副本，以便`primary`可以验证其是否与`primary`的一致（或知道将告知其删除的对象）。
    如果集群在操作被`Acting Set`所有成员持久化之前故障，随后的`peering`未记录该操作，并且记录了该操作的节点是之后重新加入，则其`PG Log`与这次故障后`peering`所重建的权威日志之间的差异将记录于分歧日志（divergent history）。
    由于分歧事件（divergent event）没有记录在来自该`Acting Set`的其他`OSD`的`PG Log`中，它们不会被通知客户端已确认，丢弃它们是无害的（这样所有`OSD`的权威日志都一致）。但是，必须指示所有存储了来自分歧更新（divergent update）数据的`OSD`删除受影响的（现在被认为是伪造的）对象。
    - 询问`missing set`（对象更新记录在了`PG Log`中，但还未更新数据），这是在接受新的写入之前必须完全同步的对象列表。
 6. 此时，`primary`的`PG Log`包含了`PG`的权威日志（authoritative history），并且有足够的信息来更新`Acting Set`中的任何其他`OSD`。
 7. 如果当前`OSD Map`中`primary`的`up_thru`值小于等于`current interval`中的第一个`epoch`，则向`monitor`发送更新请求并等待，直到收到`up_thru`被更新后的`OSD Map`。
 8. 对于当前`Acting Set`的每个`OSD`：
    - 向其发送`PG Log`更新，使其`PG Log`与`primary`的`PG Log`（权威日志）一致，这可能涉及删除歧义对象（divergent object）。
    - 等待其确认`PG Log`条目已持久化。
 9. 此时，`Acting Set`中的所有`OSD`统一了所有元数据，并且（在任何未来`peering`中）将返回包含所有更新的相同`PG Log`条目。
    - 开始接受客户端写操作（因为所有`OSD`都一致接受这些对象更新）。但是请注意，如果客户端的写操作将被提升到恢复队列的最后，等当前`Acting Set`完全同步之后才将执行写入。
    - 更新`primary`本地`PG Info`中的`pg_info_t:last_epoch_started`，并指示`Acting Set`中的其他`OSD`执行相同操作。
    - 拉取`primary`没有但其他`OSD`具有的对象数据更新。为了找到所有对象的副本，可能需要查询从`pg_history_t:last_epoch_clean`（恢复完成的最后`epoch`）之后到`pg_info_t:last_epoch_started`（最后一次`peering`完成）之前的其他`past interval`中的`OSD`。
    - 将对象数据更新推送至其他缺失的`OSD`。
    从`primary`推送这些更新（而不是让副本拉取），因为这允许`primary`确保在发送更新写之前副本具有当前内容。还可以（从`primary`）通过一次读即可将数据写入多个副本，如果每个副本都自行拉取则可能需要多次读取数据。
 10. 一旦所有副本都存储了所有（在此`epoch`开始之前就存在）的对象，就可以更新`PG Info`中的`last epoch clean`，并且可以清除所有`stray`副本，允许其删除已不在`Acting Set`中的对象副本。
 在此之前不可以清除这些`stray`，因为其中一个`stray`可能拥有旧对象（其所有副本在被同步到当前`Acting Set`中的`OSD`之前就全部down）的唯一幸存副本。

# 参考
 * [PG](https://docs.ceph.com/en/latest/dev/osd_internals/pg/)
 * [PEERING](https://docs.ceph.com/en/latest/dev/peering/)
 * [LAST_EPOCH_STARTED](https://docs.ceph.com/en/latest/dev/osd_internals/last_epoch_started/)
