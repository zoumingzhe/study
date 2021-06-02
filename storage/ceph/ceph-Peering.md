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

因此，新的primary可以使用最新的`OSD Map`以及`OSD Map`的近期历史来生成一组past interval集合，以确定在peering成功之前必须询问哪些OSD。past interval集合以最`pg_info_t:last_epoch_started`为界，即知道peering完成的最近past interval。OSD发现PG存在是通过交换PG info消息，因此OSD总是有`pg_history_t:last_epoch_started`下限值。

当前PG的primary处理流程：
 1. 获取最近的`OSD Map`（以识别所有Acting Set的成员，并确认自身仍然是primary）。
 2. 生成自`pg_info_t:last_epoch_started`以来的past intervals列表。考虑那些up_thru大于第一个间隔epoch的最后一个间隔的`OSD Map`的子集；也就是说，在Acting Set变化为另一组OSD之前，peering可能已完成的子集。
 peering成功要求能够从每个past interval的Acting Set中联系至少一个OSD。
 3. 向该列表中的每个节点询问其PG info，其中包括对PG进行的最近写入，以及`pg_info_t:last_epoch_started`值。如果发现比自身拥有的`pg_info_t:last_epoch_started`更新，可以裁剪更旧的past interval并减少peering需要联系的OSD。
 4. 如果在其他OSD的PGLog中有primary没有的操作，就指示该OSD向primary发送缺失的PGLog条目，以使primary的PGLog保持最新的（包括最新的写入）。
 5. 对于当前Acting Set的每个OSD：
    - 要求其提供自`pg_info_t:last_epoch_started`以来所有的PGLog条目副本，以便primary可以验证其是否与primary的一致（或知道将告知其删除的对象）。
    如果集群在操作被Acting Set所有成员持久化之前失败，并且随后的peering未记录该操作，并且记录了该操作的节点是之后重新加入，则其PGLog与这次失败后peering所重建的权威日志之间的差异将记录于分歧日志（divergent history）。
    由于分歧事件（divergent event）没有记录在来自该Acting Set的其他OSD的PGLog中，它们不会被通知客户端已确认，丢弃它们是无害的（这样所有OSD都同意权威日志）。但是，必须指示所有存储了来自分歧更新（divergent update）数据的OSD删除受影响的（现在被认为是伪造的）对象。
    - 询问missing set（对象更新记录在了PGLog中，但还未更新数据），这是在接受新的写入之前必须完全复制的对象列表。
 6. 此时，primary的PGLog包含了PG的权威日志（authoritative history），并且有足够的信息来更新Acting Set中的任何其他OSD。
 7. 如果当前`OSD Map`中primary的up_thru值小于等于current interval中的第一个epoch，则向monitor发送更新请求并等待，直到收到up_thru被更新后的`OSD Map`。
 8. 对于当前Acting Set的每个OSD：
    - 向其发送PGLog更新，使其PGLog与primary的PGLog（权威日志）一致，这可能涉及删除歧义对象（divergent object）。
    - 等待其确认PGLog条目已持久化。
 9. 此时，Acting Set中的所有OSD都统一了所有元数据，并且（在任何未来peering中）将返回包含所有更新的相同PGLog条目。
    - 开始接受客户端写操作（因为所有OSD都一致同意接受这些对象更新）。但是请注意，如果客户端的写操作将被提升到恢复队列的最后，在当前Acting Set完全同步之后将执行写入。
    - 更新primary本地PG info中的`pg_info_t:last_epoch_started`，并指示Acting Set中的其他OSD执行相同操作。
    - 拉取primary没有但其他OSD具有的对象数据更新。为了找到所有对象的副本，可能需要查询从`pg_history_t:last_epoch_clean`（恢复完成的最后epoch）之后到`pg_info_t:last_epoch_started`（最后一次peering完成）之前的其他past interval中的OSD。
    - 将对象数据更新推送至其他缺失的OSD。从primary推送这些更新（而不是让副本拉取），因为这允许primary确保在发送更新写入之前副本具有当前内容。还可以（从primary）通过一次读即可将数据写入多个副本，如果每个副本都自行拉取则可能需要多次读取数据。
 10. 一旦所有副本都存储了所有（在此epoch开始之前就存在）的对象，就可以更新PG info中的last epoch clean，并且可以清除所有stray副本，允许其删除已不在Acting Set中的对象副本。
 在此之前不可以清除这些stray，因为其中一个stray可能拥有旧对象（其所有副本在被同步到当前Acting Set中的OSD之前就全部down）的唯一幸存副本。

The Golden Rule is that no write operation to any PG is acknowledged to a client until it has been persisted by all members of the acting set for that PG. This means that if we can communicate with at least one member of each acting set since the last successful peering, someone will have a record of every (acknowledged) operation since the last successful peering. This means that it should be possible for the current primary to construct and disseminate a new authoritative history.

It is also important to appreciate the role of the OSD map (list of all known OSDs and their states, as well as some information about the placement groups) in the peering process:

When OSDs go up or down (or get added or removed) this has the potential to affect the active sets of many placement groups.

Before a primary successfully completes the peering process, the OSD map must reflect that the OSD was alive and well as of the first epoch in the current interval.

Changes can only be made after successful peering.

Thus, a new primary can use the latest OSD map along with a recent history of past maps to generate a set of past intervals to determine which OSDs must be consulted before we can successfully peer. The set of past intervals is bounded by last epoch started, the most recent past interval for which we know peering completed. The process by which an OSD discovers a PG exists in the first place is by exchanging PG info messages, so the OSD always has some lower bound on last epoch started.

The high level process is for the current PG primary to:

get a recent OSD map (to identify the members of the all interesting acting sets, and confirm that we are still the primary).

generate a list of past intervals since last epoch started. Consider the subset of those for which up_thru was greater than the first interval epoch by the last interval epoch’s OSD map; that is, the subset for which peering could have completed before the acting set changed to another set of OSDs.

Successful peering will require that we be able to contact at least one OSD from each of past interval’s acting set.

ask every node in that list for its PG info, which includes the most recent write made to the PG, and a value for last epoch started. If we learn about a last epoch started that is newer than our own, we can prune older past intervals and reduce the peer OSDs we need to contact.

if anyone else has (in its PG log) operations that I do not have, instruct them to send me the missing log entries so that the primary’s PG log is up to date (includes the newest write)..

for each member of the current acting set:

ask it for copies of all PG log entries since last epoch start so that I can verify that they agree with mine (or know what objects I will be telling it to delete).

If the cluster failed before an operation was persisted by all members of the acting set, and the subsequent peering did not remember that operation, and a node that did remember that operation later rejoined, its logs would record a different (divergent) history than the authoritative history that was reconstructed in the peering after the failure.

Since the divergent events were not recorded in other logs from that acting set, they were not acknowledged to the client, and there is no harm in discarding them (so that all OSDs agree on the authoritative history). But, we will have to instruct any OSD that stores data from a divergent update to delete the affected (and now deemed to be apocryphal) objects.

ask it for its missing set (object updates recorded in its PG log, but for which it does not have the new data). This is the list of objects that must be fully replicated before we can accept writes.

at this point, the primary’s PG log contains an authoritative history of the placement group, and the OSD now has sufficient information to bring any other OSD in the acting set up to date.

if the primary’s up_thru value in the current OSD map is not greater than or equal to the first epoch in the current interval, send a request to the monitor to update it, and wait until receive an updated OSD map that reflects the change.

for each member of the current acting set:

send them log updates to bring their PG logs into agreement with my own (authoritative history) … which may involve deciding to delete divergent objects.

await acknowledgment that they have persisted the PG log entries.

at this point all OSDs in the acting set agree on all of the meta-data, and would (in any future peering) return identical accounts of all updates.

start accepting client write operations (because we have unanimous agreement on the state of the objects into which those updates are being accepted). Note, however, that if a client tries to write to an object it will be promoted to the front of the recovery queue, and the write willy be applied after it is fully replicated to the current acting set.

update the last epoch started value in our local PG info, and instruct other active set OSDs to do the same.

start pulling object data updates that other OSDs have, but I do not. We may need to query OSDs from additional past intervals prior to last epoch started (the last time peering completed) and following last epoch clean (the last epoch that recovery completed) in order to find copies of all objects.

start pushing object data updates to other OSDs that do not yet have them.

We push these updates from the primary (rather than having the replicas pull them) because this allows the primary to ensure that a replica has the current contents before sending it an update write. It also makes it possible for a single read (from the primary) to be used to write the data to multiple replicas. If each replica did its own pulls, the data might have to be read multiple times.

once all replicas store the all copies of all objects (that existed prior to the start of this epoch) we can update last epoch clean in the PG info, and we can dismiss all of the stray replicas, allowing them to delete their copies of objects for which they are no longer in the acting set.

We could not dismiss the strays prior to this because it was possible that one of those strays might hold the sole surviving copy of an old object (all of whose copies disappeared before they could be replicated on members of the current acting set).

# 参考
 * [PG](https://docs.ceph.com/en/latest/dev/osd_internals/pg/)
 * [PEERING](https://docs.ceph.com/en/latest/dev/peering/)
 * [LAST_EPOCH_STARTED](https://docs.ceph.com/en/latest/dev/osd_internals/last_epoch_started/)
