Ceph Config
===========

# BACKFILLING
When you add or remove Ceph OSD Daemons to a cluster, CRUSH will rebalance the cluster by moving placement groups to or from Ceph OSDs to restore balanced utilization. The process of migrating placement groups and the objects they contain can reduce the cluster’s operational performance considerably. To maintain operational performance, Ceph performs this migration with ‘backfilling’, which allows Ceph to set backfill operations to a lower priority than requests to read or write data.

当向集群添加或删除Ceph OSD守护程序时，CRUSH算法将通过在Ceph OSD之间移动PGs来重新平衡集群，以使利用率恢复平衡。迁移PGs及其所包含对象的过程会显着降低集群的运行性能。为了保持性能，Ceph使用“backfilling”执行此迁移，这允许Ceph将回填操作设置为低于数据读写请求的优先级。

## osd_max_backfills
The maximum number of backfills allowed to or from a single OSD. Note that this is applied separately for read and write operations.

单个OSD允许的最大回填次数。请注意，这分别适用于读取和写入操作。

## osd_backfill_scan_min
The minimum number of objects per backfill scan.

每次回填扫描的最小对象个数。

## osd_backfill_scan_max
The maximum number of objects per backfill scan.

每次回填扫描的最大对象个数。

## osd_backfill_retry_interval
The number of seconds to wait before retrying backfill requests.

重试回填请求前等待的秒数。

# RECOVERY
When the cluster starts or when a Ceph OSD Daemon crashes and restarts, the OSD begins peering with other Ceph OSD Daemons before writes can occur. See Monitoring OSDs and PGs for details.

If a Ceph OSD Daemon crashes and comes back online, usually it will be out of sync with other Ceph OSD Daemons containing more recent versions of objects in the placement groups. When this happens, the Ceph OSD Daemon goes into recovery mode and seeks to get the latest copy of the data and bring its map back up to date. Depending upon how long the Ceph OSD Daemon was down, the OSD’s objects and placement groups may be significantly out of date. Also, if a failure domain went down (e.g., a rack), more than one Ceph OSD Daemon may come back online at the same time. This can make the recovery process time consuming and resource intensive.

To maintain operational performance, Ceph performs recovery with limitations on the number recovery requests, threads and object chunk sizes which allows Ceph perform well in a degraded state.

当集群启动或Ceph OSD守护进程崩溃并重新启动时，OSD会在写入之前与其他Ceph OSD守护进程进行peering。有关详细信息请参阅`Monitoring OSDs and PGs`。

## osd_recovery_delay_start

## osd_recovery_max_active
The number of active recovery requests per OSD at one time. More requests will accelerate recovery, but the requests places an increased load on the cluster.

每个OSD单次可以处理的active恢复请求个数。更多的请求将加速恢复，但会增加集群的负载。

## osd_recovery_max_active_hdd
The number of active recovery requests per OSD at one time, if the primary device is rotational.

## osd_recovery_max_active_ssd
The number of active recovery requests per OSD at one time, if the primary device is non-rotational (i.e., an SSD).

## osd_recovery_max_single_start
The maximum number of recovery operations per OSD that will be newly started when an OSD is recovering.

当某个OSD恢复时，每个新启动的OSD的最大恢复操作个数。

## osd_recovery_sleep
Time in seconds to sleep before the next recovery or backfill op. Increasing this value will slow down recovery operation while client operations will be less impacted.

在下一次恢复或回填操作之前的睡眠时间（以秒为单位）。增加此值将减慢恢复操作，而客户端操作受影响较小。

## osd_recovery_sleep_hdd
Time in seconds to sleep before next recovery or backfill op for HDDs.

使用HDD时，在下一次恢复或回填操作之前的睡眠时间（以秒为单位）。

## osd_recovery_sleep_ssd
Time in seconds to sleep before the next recovery or backfill op for SSDs.

使用SSD时，在下一次恢复或回填操作之前的睡眠时间（以秒为单位）。

## osd_recovery_sleep_hybrid
Time in seconds to sleep before the next recovery or backfill op when OSD data is on HDD and OSD journal / WAL+DB is on SSD.

当数据在HDD上且journal/WAL+DB在SSD上时，在下一次恢复或回填操作之前的睡眠时间（以秒为单位）。

## osd_recovery_priority
The default priority set for recovery work queue. Not related to a pool’s recovery_priority.

为恢复工作队列设置的默认优先级。与池的`recovery_priority`无关。

# 参考
 * [OSD CONFIG REFERENCE](https://docs.ceph.com/en/latest/rados/configuration/osd-config-ref/)
