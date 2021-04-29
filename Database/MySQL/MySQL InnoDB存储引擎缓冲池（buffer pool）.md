# MySQL InnoDB存储引擎缓冲池（buffer pool）
MySQL作为一个存储系统，具有缓冲池（buffer pool）机制，以避免每次查询数据都进行磁盘IO。

# 什么是预读？
磁盘并不是按需读取，而是按页读取，一次至少读一页数据，如果未来要读取的数据就在当前页中，就能够省去后续的磁盘IO，提高效率。

数据访问通常都遵循“集中读写”的原则，使用一些数据时大概率也会使用附近的数据，这就是所谓的“局部性原理”。

预读机制表明，按页读取磁盘能够提高性能、减少磁盘IO，所以可以按页加载磁盘数据并缓存至缓冲池。

# 什么是预读失效？
由于预读（Read-Ahead）提前把页放入了缓冲池，但最终并没有从页中读取数据，称为预读失效。

**如何优化预读失效？**

（1）让预读失败的页，停留在缓冲池LRU里的时间尽可能短。

（2）让真正被读取的页，才挪到缓冲池LRU的头部。

# 什么是缓冲池污染？
当某一个SQL语句执行大量数据的批量扫描时，可能导致缓冲池的大量页换出，热数据被淘汰引起性能急剧下降，这种情况叫缓冲池污染。

例如，有一个数据量较大的用户表，当执行SQL语句“select * from user where name like "%shenjian%";”时，虽然结果集可能只有少量数据，但这类like不能命中索引，必须全表扫描，就需要访问大量的页：

（1）把页加到缓冲池（插入old sublist头部）。

（2）从页里读出相关的row（插入new sublist头部）。

（3）row里的name字段和字符串shenjian进行比较，如果符合条件，加入到结果集中。

（4）直到扫描完所有页中的所有row。

**怎么解决缓冲池污染问题？**

（1）增加old sublist停留时间窗口机制。

（2）插入old sublist头部的页，即使立刻被访问，并不会立刻放入new sublist头部。

（3）只有满足“被访问”并且“在old sublist停留时间”大于T，才会被放入new sublist头部。

# 参数设置
 - [innodb_buffer_pool_size](./MySQL参数.md#innodb_buffer_pool_size)
 - [innodb_old_blocks_pct](./MySQL参数.md#innodb_old_blocks_pct)
 - [innodb_old_blocks_time](./MySQL参数.md#innodb_old_blocks_time)

# 参考
1.[缓冲池(buffer pool)，这次彻底懂了！！！](https://blog.csdn.net/shenjian58/article/details/93268633)
