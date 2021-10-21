# MySQL InnoDB存储引擎写缓冲（change buffer）
对于读请求，[缓冲池（buffer pool）](./MySQL%20InnoDB存储引擎缓冲池（buffer%20pool）.md)能够缓存热数据，减少磁盘IO，提升性能。

那么对于写请求又该如何提升其性能呢？

 + 场景1：访问页缓冲池命中
 
 （1）直接修改缓冲池中的页，一次内存操作。

 （2）写入redo log，一次磁盘顺序写操作。

 这样做的效率最高。

 + 场景2：访问页缓冲池缺失

 （1）加载磁盘页到缓冲池，一次磁盘随机读操作。

 （2）修改缓冲池中的页，一次内存操作。

 （3）写入redo log，一次磁盘顺序写操作。

这样做在缓冲池缺失时将至少产生一次磁盘IO，对于写多读少的业务场景，是否可以优化呢？

 + 场景3：访问页缓冲池缺失，增加写缓冲

 （1）在写缓冲中记录这个操作，一次内存操作。

 （2）写入redo log，一次磁盘顺序写操作。

# 什么是InnoDB的写缓冲？
在MySQL5.5之前叫插入缓冲(insert buffer)，只针对insert做了优化；MySQL5.5加入了写缓冲(change buffer)，对delete和update也有效。

写缓冲是对非唯一普通索引页（non-unique secondary index page）进行写操作时出现缓冲池缺失的一种优化，缓冲池缺失时并不会立即将磁盘页加载到缓冲池，而是仅仅记录缓冲变更（buffer changes），等未来数据被读取时或定期的将数据合并（merge）恢复到缓冲池中的技术。

写缓冲的目的是降低写操作的磁盘IO，提升数据库性能。

# 哪些场景会触发写缓冲中的数据合并？
## 被动merge
 - 数据页被访问
 - redo log写满时
## 主动merge
 - 数据库定期或空闲时
 - 数据库写缓冲过多时
 - 数据库shutdown时

# 为什么写缓冲优化不适用于唯一普通索引页？
如果索引设置为唯一索引（unique index），在进行写操作时InnoDB必须进行唯一性检查。也就是说，索引页缓冲缺失时无法避免加载磁盘页，否则无法进行唯一性校验。此时就应该直接在缓冲页进行修改，而不应该再写缓冲。

## 写缓冲不适合的场景？
 - 数据库都是唯一索引。
 - 写入一个数据后，会立刻读取它。

这两类场景，在写操作进行时（或进行后）就要读取缓冲页，此时写缓存变成了额外开销，增加了复杂度。

## 写缓冲适合的场景？
 - 数据库大部分是非唯一索引。
 - 业务是写多读少，或者不是写后立刻读取。

例如，账单流水业务。

# 参数设置
 - [innodb_change_buffer_max_size](./MySQL参数.md#innodb_change_buffer_max_size)
 - [innodb_change_buffering](./MySQL参数.md#innodb_change_buffering)

# 参考
1.[写缓冲(change buffer)，这次彻底懂了！！！](https://blog.csdn.net/shenjian58/article/details/93691224)
