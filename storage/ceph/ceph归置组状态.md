Ceph Placement Group States （归置组状态）
=========================================

## creating
```
Ceph is still creating the placement group.
Ceph仍在创建归置组。
```

## activating
```
The placement group is peered but not yet active.
归置组已经互联，但尚未active。
```

## active
```
Ceph will process requests to the placement group.
Ceph可处理对归置组的请求。
```

## clean
```
Ceph replicated all objects in the placement group the correct number of times.
Ceph将归置组内的所有对象复制了规定次数。
```

## down
```
A replica with necessary data is down, so the placement group is offline.
一个包含必备数据的副本挂了，所以归置组处于离线状态。
```

## laggy
```
A replica is not acknowledging new leases from the primary in a timely fashion; IO is temporarily paused.
一个副本未及时确认来自主副本的新租约，IO暂时暂停。
```

## wait
```
The set of OSDs for this PG has just changed and IO is temporarily paused until the previous interval’s leases expire.
此归置组的OSD集刚刚更改，IO暂时暂停，直到上一个间隔的租约到期。
```

## scrubbing
```
Ceph is checking the placement group metadata for inconsistencies.
Ceph正在检查归置组元数据的一致性。
```

## deep
```
Ceph is checking the placement group data against stored checksums.
Ceph正在检查归置组数据和所存checksums之间的一致性。
```

## degraded
```
Ceph has not replicated some objects in the placement group the correct number of times yet.
Ceph尚未将归置组内的某些对象复制到规定次数。
```

## inconsistent
```
Ceph detects inconsistencies in the one or more replicas of an object in the placement group (e.g. objects are the wrong size, objects are missing from one replica after recovery finished, etc.).
Ceph检测到了归置组中对象的一或多个副本间不一致（例如：对象的大小错误、恢复完成后一个副本中缺少对象、等等）。
```

## peering
```
The placement group is undergoing the peering process
归置组正在互联。
```

## repair
```
Ceph is checking the placement group and repairing any inconsistencies it finds (if possible).
Ceph正在检查归置组并修复发现的任何不一致（如果可能的话）。
```

## recovering
```
Ceph is migrating/synchronizing objects and their replicas.
Ceph正在迁移/同步对象及其副本。
```

## forced_recovery
```
High recovery priority of that PG is enforced by user.
用户强制指定了该归置组高恢复优先级。
```

## recovery_wait
```
The placement group is waiting in line to start recover.
归置组正在排队等待开始恢复。
```

## recovery_toofull
```
A recovery operation is waiting because the destination OSD is over its full ratio.
恢复操作因目标OSD已超过其占满率发生等待。
```

## recovery_unfound
```
Recovery stopped due to unfound objects.
恢复因找不到对象而停止。
```

## backfilling
```
Ceph is scanning and synchronizing the entire contents of a placement group instead of inferring what contents need to be synchronized from the logs of recent operations. Backfill is a special case of recovery.
Ceph正在扫描和同步整个归置组的全部内容，而不是从最近操作的日志推断哪些内容需要同步。
Backfill（回填）是恢复的一种特殊情况。
```

## forced_backfill
```
High backfill priority of that PG is enforced by user.
用户强制指定了该归置组高回填优先级。
```

## backfill_wait
```
The placement group is waiting in line to start backfill.
归置组正在排队等待开始回填。
```

## backfill_toofull
```
A backfill operation is waiting because the destination OSD is over the backfillfull ratio.
恢复操作因目标OSD已超过其回填占满率发生等待。
```

## backfill_unfound
```
Backfill stopped due to unfound objects.
回填因找不到对象而停止。
```

## incomplete
```
Ceph detects that a placement group is missing information about writes that may have occurred, or does not have any healthy copies. If you see this state, try to start any failed OSDs that may contain the needed information. In the case of an erasure coded pool temporarily reducing min_size may allow recovery.
Ceph检测到归置组丢失了可能已发生的写入信息，或者没有任何健康的副本。
如果您看到此状态，尝试启动任何可能包含所需信息的失败OSD。
如果是纠删码存储池，临时减少min_size也可能允许恢复。
```

## stale
```
The placement group is in an unknown state - the monitors have not received an update for it since the placement group mapping changed.
归置组处于未知状态，监视器在归置组映射更改后未收到更新。
```

## remapped
```
The placement group is temporarily mapped to a different set of OSDs from what CRUSH specified.
归置组被临时映射到与CRUSH所指定的OSD集合不同的集合。
```

## undersized
```
The placement group has fewer copies than the configured pool replication level.
归置组的副本数少于配置的存储池副本数。
```

## peered
```
The placement group has peered, but cannot serve client IO due to not having enough copies to reach the pool’s configured min_size parameter. Recovery may occur in this state, so the pg may heal up to min_size eventually.
归置组已互联，但是不能为客户端IO提供服务，因为其副本数没达到存储池配置的min_size参数。
在此状态下可能会发生恢复，因此归置组最终可能恢复到min_size。
```

## snaptrim
```
Trimming snaps.
删除快照。
```

## snaptrim_wait
```
Queued to trim snaps.
删除快照。
```

## snaptrim_error
```
Error stopped trimming snaps.
删除快照出错停止。
```

## unknown
```
The ceph-mgr hasn’t yet received any information about the PG’s state from an OSD since mgr started up.
ceph-mgr自从启动以来还未从OSD接收到关于归置组状态的任何信息。
```

# 参考
 * [Ceph中一些PG相关的状态说明和基本概念说明](https://www.ichenfu.com/2018/07/06/ceph-pg-states-concepts/)
 * [Placement Group States](https://github.com/ceph/ceph/blob/v14.0.0/doc/rados/operations/pg-states.rst)
 * [PLACEMENT GROUP STATES](https://docs.ceph.com/en/latest/rados/operations/pg-states/)
 * [归置组状态](http://docs.ceph.org.cn/rados/operations/pg-states/)
