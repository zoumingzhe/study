Ceph 对象版本
=============
Ceph对象版本由Eversion(epoch, version)组成：其中epoch是OSDMap（./src/osd/OSDMap.h）的版本号，每当有OSD状态变化（如：增加、删除）时又OSDMonitor递增；version是PGMap（./src/mon/PGMap.h）的版本号，每次更新操作时由Primary OSD递增。

./src/osd/OSD.h
```
// NOTE: these must match ceph_fs.h typedefs
typedef uint64_t ceph_tid_t; // transaction id
typedef uint64_t version_t;
typedef __u32 epoch_t;       // map epoch  (32bits -> 13 epochs/second for 10 years)
```

# epoch
 - Epoch 元版本
```
A (monotonically increasing) OSD map version number.
```
一个（单调递增的）OSD map版本号。

 - Last Epoch Start 最近起始元版本
```
The last epoch at which all nodes in the Acting Set for a particular placement group agreed on an Authoritative History. At this point, Peering is deemed to have been successful.
```
特定归置组Acting Set内的所有节点都对Authoritative History达成了一致的最近元版本。此版本被认为已经成功互联。

 - Last Epoch Clean 最近干净元版本
```
The last Epoch at which all nodes in the Acting set for a particular placement group were completely up to date (both placement group logs and object contents). At this point, recovery is deemed to have been completed.
```
特定归置组Acting Set内的所有节点都全部更新（包括PG日志和对象内容）的最近元版本。此版本被认为已完成恢复。
```
class LastEpochClean {
    struct Lec {
      vector<epoch_t> epoch_by_pg;
      ps_t next_missing = 0;
      epoch_t floor = std::numeric_limits<epoch_t>::max();
      void report(ps_t pg, epoch_t last_epoch_clean);
    };
    std::map<uint64_t, Lec> report_by_pool;
  public:
    void report(const pg_t& pg, epoch_t last_epoch_clean);
    void remove_pool(uint64_t pool);
    epoch_t get_lower_bound(const OSDMap& latest) const;
};
```

# version
Ceph在磁盘上维护两个版本信息：eversion_t pg_log.head和version_t info.user_version。每个对象都用最后修改时的PG version和user_version标记版本。通过操作OpContext::at_version修改PG version，然后将其作为事务持久化到PG Log中，并在递增已使用位置。通过操作新的OpContext::user_at_version（./src/osd/PrimaryLogPG.cc）修改user_version，并且也通过PG Log事务持久化。当op为“用户修改”（非监视写入）时，只会在PrimaryLogPG::prepare_transaction（./src/osd/PrimaryLogPG.cc）中修改user_at_version，持久化user_version会按照如下规则进行更新：
 1. 设置user_at_version为ctx->new_obs.oi.user_version与info.last_user_version之间的最大值+1。
 2. 设置user_at_version为自身与ctx->at_version.version之间的最大值。
 3. ctx->new_obs.oi.user_version = ctx->user_at_version （更改对象的user_version）。

./src/osd/PrimaryLogPG.cc@PrimaryLogPG::finish_ctx
```
  // finish and log the op.
  if (ctx->user_modify) {
    // update the user_version for any modify ops, except for the watch op
    ctx->user_at_version = MAX(info.last_user_version, ctx->new_obs.oi.user_version) + 1;
    /* In order for new clients and old clients to interoperate properly
     * when exchanging versions, we need to lower bound the user_version
     * (which our new clients pay proper attention to)
     * by the at_version (which is all the old clients can ever see). */
    if (ctx->at_version.version > ctx->user_at_version)
      ctx->user_at_version = ctx->at_version.version;
    ctx->new_obs.oi.user_version = ctx->user_at_version;
  }
```

这组更新语义意味着：对于传统的池，user_version将等于过去的resert_version；对于缓存池，object和PG user-version将能够跨池，而不会造成错乱。为了支持旧客户端，依旧保留了原有的resert_version，但将其重命名为“bad_replay_version”，并且像以前一样为它赋值：for writes设置为at_version（并且是正确的回放版本）；for watches设置为user version；for ENOENT replies设置为重放版本的epoch，而非user_version版本。并且现在read ops和write ops也会为bad_replay_version的version_t部分赋值，这对于所有的旧客户端都应该没问题。

对于新客户端，则会阻止其读取bad_replay_version，并为其添加两个适当的成员（user_version和replay_version）；每次操作（包括读操作）都会为user_version赋值，而写操作则会为replay_version赋值。

objclass中的get_current_version()函数现在总是返回pg->info.last_user_version，这意味着它保证包含PG中最后一次用户更新的版本（包括读取时的版本）。


# 参考
 * [PLACEMENT GROUP CONCEPTS](https://docs.ceph.com/en/latest/rados/operations/pg-concepts/)
 * [PUBLIC OSD VERSION](https://docs.ceph.com/en/latest/dev/versions/#public-osd-version)
