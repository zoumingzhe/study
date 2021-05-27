Ceph up_thru
============
思考下类极端场景：

Ceph集群中某个PG有两个osd（`osd.1`、`osd.2`），在某个时刻`osd.1`和`osd.2`都是最新副本，此时`osd.1`如果`down`了，随后`osd.2`也`down`了，接着`osd.1`又`up`了，那么此时`osd.1`是否能提供读写服务呢？

|        | osd.1  | osd.2  |
| :----: | :----: | :----: |
| epoch1 | **up** | **up** |
| epoch2 |~~down~~| **up** |
| epoch3 |~~down~~|~~down~~|
| epoch3 | **up** |~~down~~|

在`osd.1`处于`down`期间：
 - 如果`osd.2`有数据更新，那么`osd.1`再次`up`后显然不能提供读写服务的，因为其上的副本不是最新的。
 - 如果`osd.2`没有数据更新，那么`osd.1`再次`up`后是可以提供读写服务的。

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

通过数据结构可以知道，up_thru是保存在OSDMap里的一个（epoch_t类型）版本号。

在OSD处于alive + healthy阶段通常会跟踪两个interval：
 - 最近的是[up_from, up_thru)，这里up_thru（如果 > up_from）是已知OSD处于`_started_`状态时的最后一个epoch，即OSD实际down的下限，而down_at（如果 > up_from）则是OSD实际down的上限。
 - 另一个是last_clean interval[begin，end)，这时last interval是已知OSD处于`_finished_`状态时或cleanly shut down的最后epoch。在可能的情况下，需将其向前推进到OSD最终epoch。

lost_at用于允许build_prior继续进行而无需等待OSD恢复。在某些情况下，进程可能会被阻塞，因为OSD处于down状态时可能包含更新（例如：PG在某个时间间隔内可能已经在读写）。如果OSD无法回到online状态，可以在知道丢失某些已确认的写入的情况下迫使事情继续进行。即使OSD稍后恢复正常，但是那些写操作仍然会丢失（the divergent objects will be thrown out）。


we track up to two intervals during which the osd was alive and healthy.  the most recent is [up_from,up_thru), where up_thru is the last epoch the osd is known to have _started_.  i.e., a lower bound on the actual osd death.  down_at (if it is > up_from) is an upper bound on the actual osd death.

the second is the last_clean interval [begin,end).  in that case, the last interval is the last epoch known to have been either _finished_, or during which the osd cleanly shut down.  when possible, we push this forward to the epoch the osd was eventually marked down.

the lost_at is used to allow build_prior to proceed without waiting for an osd to recover.  In certain cases, progress may be blocked because an osd is down that may contain updates (i.e., a pg may have gone rw during an interval).  If the osd can't be brought online, we can force things to proceed knowing that we _might_ be losing some acked writes.  If the osd comes back to life later, that's fine to, but those writes will still be lost (the divergent objects will be thrown out).

通过`ceph osd dump`查看OSDMap信息如下：
```shell
osd.1 up   in  weight 1 up_from 12771 up_thru 13289 down_at 12764 last_clean_interval [12742,12763) ...
osd.2 up   in  weight 1 up_from 12770 up_thru 13289 down_at 12764 last_clean_interval [12741,12763) ...
```

# up_thru的来龙去脉

# 参考
 * [聊聊Ceph之up_thru来龙去脉分析](https://zhuanlan.zhihu.com/p/165471449)
 * [ceph之up_thru分析](https://ivanzz1001.github.io/records/post/ceph/2017/07/21/ceph-up_thru)
 * [ceph peering机制-过程分析](https://blog.51cto.com/wendashuai/2513897)
