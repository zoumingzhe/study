Ceph 数据修复
=============
当PG完成了Peering过程后，处于Active状态的PG就可以对外提供服务了。如果PG的各个副本出现不一致，则需要进行修复。ceph的修复过程有两种：Recovery和Backfill。

Recovery是仅依据PG日志中的缺失记录来修复不一致的对象。Backfill是PG通过重新扫描所有的对象，对比发现缺失的对象，通过整体拷贝来修复。当一个OSD失效时间过长导致无法根据PG日志来修复，或者新加入的OSD导致数据迁移时，就会启动Backfill过程。

如果需要Recovery，就产生DoRecovery事件，触发修复操作；如果需要Backfill，就会产生RequestBackfill事件来触发Backfill操作。

PG的修复过程中，如果既有需要Recovery过程的OSD，又有需要Backfill过程的OSD，那么处理过程需要先进行Recovery过程的修复，再完成Backfill过程的修复。

# LOG BASED PG
目前，所有Ceph池类型的一致性都是由primary log-based replication来保证的，这适用于纠删码（EC）池和多副本池。

## PRIMARY LOG-BASED REPLICATION
读操作必须返回由任何已完成的写操作写入的数据（客户端可能已收到提交消息）。有很多方法可以解决此问题，但是Ceph的体系结构使每个map epoch的每个人都很容易知道主要谁是primary。因此，简单的答案是将特定PG的所有写操作由单个primary排序后路由至replicas。尽管实际上只需要序列化单个RADOS对象上的写操作（即使那样，部分排序实际也只需要提供在重叠区域上的写操作之间的排序），但是也可以序列化整个PG上的写入，因为它可以只使用两个数字表示PG的当前状态：最近一次写入开始时primary上的map epoch（由于map分发本身是异步的，因此这看起来有些奇怪，请参阅“Peering”和“the concept of interval changes”）以及每个PG不断增加的版本号（在代码中使用`eversion_t`类型进行引用，并以`pg_info_t::last_update`的形式存储）。此外，维护“最近”操作的日志，该日志至少可以扩展到包含任何不稳定（已开始但尚未提交）的写入和本地未更新的对象（请参阅“recovery”和“backfil”）。实际上，日志会扩展得更远（清理时为`osd_min_pg_log_entries`，不清理时为`osd_max_pg_log_entries`），因为它便于快速执行恢复。

使用此日志，只要我们与OSD的非空子集进行对话，而该子集必须已接受我们接受写操作的最近间隔内的所有已完成写操作，则可以确定一个保守日志，该日志必须包含已向客户端报告为已提交的任何写操作。这里有一些自由，可以在该集合的某个元素记住的最旧头（没有包含该日志的任何较新的记录都不能完成）和记住的最新头（显然，日志中的所有写操作都已启动，因此我们可以记住它们）之间进行选择作为新头。这是`PG/PrimaryLogPG`中多副本池和纠删码池之间的主要区别：多副本池尝试选择最新的有效选项，以避免客户端需要重放这些操作并恢复其他副本。相反，纠删码池尝试选择可用的最旧的选项。

这样做的原因涉及到实现中其他差异的核心：一个副本通常不足以重建EC对象。事实上，在某些编码中，某些日志组合会留下无法恢复的对象（例如：k=4，m=2编码，其中三个副本记了一个写操作，而其他三个副本没有记该写操作–这两个版本都没有3个副本）。因此，代表不稳定写操作（尚未提交给客户端的写操作）的日志条目必须只能使用纠删码池上的本地信息进行回滚。因此，日志条目通常可以被回滚的（在这种情况下，通过延迟的应用程序或通过一组用于回滚就地更新的指令），也可以不被回滚。多副本池日志条目则永远无法回滚。

更多详细信息，请参见`PGLog.h/cc`, `osd_types.h:pg_log_t`, `osd_types.h:pg_log_entry_t`和“peering”。

## PGBackend
多副本和纠删码之间的根本区别在于多副本可以进行破坏性更新，而擦除编码则不能。如果针对多副本和纠删码分别实现不同的完整PrimaryLogPG则会变得繁琐，因为实际上仅有几个基本的不同：
 1. 读取方式：仅异步，EC需要远程读
 2. 写入方式：要么限制为append，要么必须write aside并执行tpc
 3. 在互联过程中，是选择最早的条目还是最新的条目
 4. 日志条目中的一些额外的信息可支持回滚
并有许多相似之处：
 1. 对象的所有统计数据和元数据
 2. 用于将客户端IO与recovery和scrub混合的高级锁定规则
 3. 用于在不暴露未提交状态下混合读写的高级锁定规则（稍后可能回滚或忘记）
 4. 确定参与接受写入的最近时间间隔的osd集合所需的进程、元数据和协议
取而代之的是，选择一些抽象（和一些kludge）来说明差异：
 1. `PGBackend`
 2. `PGTransaction`
 3. `PG::choose_acting` chooses between `calc_replicated_acting` and `calc_ec_acting`
 4. 写管道的不同位禁止基于池类型的某些操作（例如：omap操作、类操作的读和写），这些操作（目前为止）都没有对EC进行对齐
 5. Misc other kludges here and there
`PGBackend`和`PGTransaction`允许抽象上述差异1和2，并根据需要向日志条目添加4。

多副本的实现位于`./src/osd/ReplicatedBackend.h(.cc)`中，不需要过多的解释。关于`ECBackend`的更多细节可以在`./doc/dev/osd_internals/erasure_coding/ecbackend.rst`中找到。

## PGBACKEND INTERFACE EXPLANATION
注意：这是一个Firefly版本之前的设计文档，一些方法名可能已经过时了。

### READABLE VS DEGRADED
对于多副本池，如果对象存在于primary（正确版本）上，则该对象是可读的。对于纠删码池，至少需要m个分片来执行读取，并且需要在primary上进行读操作。因此，PGBackend需要包含一些接口来确定何时需要恢复才能提供读与写服务。这也改变了互联时有足够的日志来证明它的规则。

Core Changes:
 - `PGBackend`需要能够返回`IsPG(Recoverable|Readable)Predicate`对象，以使用户能够做出这些决定。

### CLIENT READS
primary OSD始终可以同步满足从多副本池中进行的读取。在纠删码池中，primary需要从一定数量的副本中请求数据才能满足读取要求。因此，`PGBackend`需要提供单独的`objects_read_sync`和`objects_read_async`接口，而`ECBackend`不会实现前者。

`PGBackend` interfaces:
 - `objects_read_sync`
 - `objects_read_async`

### SCRUBS
当前，有两种具有不同默认频率的scrub模式：
 1. [shallow] scrub：比较对象集合、元数据，但不比较内容
 2. deep scrub：比较对象集合、元数据和对象内容（包括omap）的CRC32

primary从每个副本中请求特定对象范围的scrubmap。副本会为对象范围填充此scrubmap，包括每个对象内容的CRC32（如果deep scrub的话）。primary从每个副本中收集这些scrubmap，并执行比较以识别不一致的对象。

注意：`PGBackend`实现必须负责实际执行扫描，因此对于纠删码的PG，大多数操作基本上都可以保持不变。

`PGBackend` interfaces:
 - `be_*`

### RECOVERY
恢复对象的逻辑取决于后端。使用多副本策略，首先将对象副本拉到primary，然后将其同时推送到副本。使用纠删码策略，可能希望读取重建对象所需的最小副本块数，并同时推送替换块。

另一个区别是，纠删码PG中的对象可能是不可恢复的，而不是找不到的。`unfound`状态可能应该重命名为`unrecoverable`。此外，`PGBackend`实现必须能够直接搜索具有不可恢复对象块的PG副本，并且能够确定特定对象是否可恢复。
Core changes:
 - `s/unfound/unrecoverable`

PGBackend interfaces:
 - on_local_recover_start
 - on_local_recover
 - on_global_recover
 - on_peer_recover
 - begin_peer_recover

# 一致性校验

# 资源预约
在数据修复的过程中，为了控制一个OSD上正在修复的PG最大数目，需要资源预约，在主OSD上和从OSD上都需要预约。如果没有预约成功，需要阻塞等待。一个OSD能同时修复的最大PG数在配置选项osd_max_backfills中设置，默认值为1。

类AsyncReserver(src/common/AsyncReserver.h)用来管理资源预约，其模板参数<T>为要预约的资源类型。该类实现了异步的资源预约。当成功完成资源预约后，就调用注册的回调函数通知调用方预约成功：
```C++
template <typename T>
class AsyncReserver {
  CephContext *cct;
  Finisher *f;               //当预约成功，用来执行的回调函数
  unsigned max_allowed;      //定义允许的最大资源数量，在这里指允许修复的PG的数量
  unsigned min_priority;     //最小的优先级
  Mutex lock;

  ···

  //优先级到待预约资源链表的映射，pair<T, Context *>定义预约的资源和注册的回调函数
  map<unsigned, list<Reservation>> queues;

  //资源在queues链表中的位置指针
  map<T, pair<unsigned, typename list<Reservation>::iterator>> queue_pointers;

  //预约成功，正在使用的资源
  map<T,Reservation> in_progress;

  set<pair<unsigned,T>> preempt_by_prio;  ///< in_progress that can be preempted
};
```

## 请求预约
./src/common/AsyncReserver.h:request_reservation()函数用于请求预约资源：
```C++
/**
* Requests a reservation
*
* Note, on_reserved may be called following cancel_reservation.  Thus,
* the callback must be safe in that case.  Callback will be called
* with no locks held.  cancel_reservation must be called to release the
* reservation slot.
*/
void request_reservation(
  T item,                   ///< [in] reservation key
  Context *on_reserved,     ///< [in] callback to be called on reservation
  unsigned prio,            ///< [in] priority
  Context *on_preempt = 0   ///< [in] callback to be called if we are preempted (optional)
  ) {
    Mutex::Locker l(lock);
    Reservation r(item, prio, on_reserved, on_preempt);
    rdout(10) << __func__ << " queue " << r << dendl;
    assert(!queue_pointers.count(item) &&
        !in_progress.count(item));
    queues[prio].push_back(r);
    queue_pointers.insert(make_pair(item, make_pair(prio,--(queues[prio]).end())));
    do_queues();
}
```

具体处理过程如下：
 1. 把要请求的资源根据优先级添加到queue队列中，并在queue_pointers中添加其对应的位置指针：
```C++
queues[prio].push_back(r);
queue_pointers.insert(make_pair(item, make_pair(prio,--(queues[prio]).end())));
```
 2. 调用函数do_queues()用来检查queue中的所有资源预约申请：从优先级高的请求开始检查，如果还有配额并且其请求的优先级至少不小于最小优先级，就把资源授权给它。
 3. 在queue队列中删除该资源预约请求，并在queue_pointers删除该资源的位置信息。把该资源添加到in_progress队列中，并把请求相应的回调函数添加到Finisher类中，使其执行该回调函数。
 4. 最后通知预约成功。

## 取消预约
./src/common/AsyncReserver.h:cancle_reservation()函数用于释放拥有的不再使用的资源：
```C++
/**
* Cancels reservation
*
* Frees the reservation under key for use.
* Note, after cancel_reservation, the reservation_callback may or
* may not still be called. 
*/
void cancel_reservation(
  T item                   ///< [in] key for reservation to cancel
  ) {
    Mutex::Locker l(lock);
    auto i = queue_pointers.find(item);
    if (i != queue_pointers.end()) {
      unsigned prio = queue_pointers[item].first;
      const Reservation& r = *i->second.second;
      rdout(10) << __func__ << " cancel " << r << " (was queued)" << dendl;
      delete r.grant;
      delete r.preempt;
      queues[prio].erase(i->second.second);
      if (queues[prio].empty()) {
        queues.erase(prio);
      }
      queue_pointers.erase(i);
    } else {
      auto p = in_progress.find(item);
      if (p != in_progress.end()) {
        rdout(10) << __func__ << " cancel " << p->second
                  << " (was in progress)" << dendl;
        if (p->second.preempt) {
          preempt_by_prio.erase(std::make_pair(p->second.prio, p->second.item));
          delete p->second.preempt;
        }
        in_progress.erase(item);
      } else {
        rdout(10) << __func__ << " cancel " << item << " (not found)" << dendl;
      }
    }
    do_queues();
}
```

具体处理过程如下：
 1. 首先在queue队列中查找该资源，如果在就从queue_pointers中删除该资源（这属于异常情况的处理，会有打印信息）；
 2. 如果该资源不在queue_pointers中，就从in_progress队列中查找该资源，如果在就从in_progress队列中删除该资源；
 3. 如果该资源既不在queue_pointers中，又不在in_progress队列中，则打印"(not found)"信息；
 4. 最后，调用do_queues()函数把该资源重新授权给其他等待的请求。

# 数据修复
数据修复过程状态转换图如下图所示。当PG进入Active状态后，就进入默认的子状态Activating：

![数据修复状态转换图](./ceph数据修复/数据修复状态转换图.png)

## Recovery过程

## Backfill过程

# 参考
 * [ceph数据修复](https://ivanzz1001.github.io/records/post/ceph/2019/02/02/ceph-src-code-part11_1)
 * [Ceph中的数据一致性](https://blog.shunzi.tech/post/ceph-consistency/)
 * [PGLog写流程梳理](https://blog.csdn.net/Z_Stand/article/details/100082984)
 * [Ceph源码解析：Scrub故障检测](https://www.cnblogs.com/chenxianpao/p/5878159.html)
