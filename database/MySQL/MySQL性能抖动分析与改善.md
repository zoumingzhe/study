# MySQL性能抖动分析与改善

# MySQL性能抖动分析
## redo日志对MySQL性能影响
 1. InnoDB在处理更新、插入、删除等语句时，在修改data前先记redo日志（WAL机制），在redo日志落盘之后就返回客户端成功。所以，redo日志写性能对MySQL性能很大，并且redo日志具有顺序写特性。
 2. 当InnoDB将redo日志文件写满时，它必须在checkpoint时将buffer pool中的脏页刷盘。redo日志文件越大，buffer pool的checkpoint刷盘就越少，从而节省磁盘I/O。redo日志文件过小会导致许多不必要的磁盘写入，通常redo日志空间至少需要足够处理一小时的写活动。

### redo日志切换时间
通过查看创建时间与最后修改时间，即可计算redo日志切换时间。
```shell
ls -l /mydata/ib_logfile*
```

# MySQL性能抖动改善
## 优化redo日志
 1. 修改[innodb_log_group_home_dir](./MySQL参数.md#innodb_log_group_home_dir)路径，使用高性能存储以提高redo日志文件写性能，并避免潜在的I/O资源冲突。
 2. 使redo日志文件组合大小（[innodb_log_file_size](./MySQL参数.md#innodb_log_file_size) * [innodb_log_files_in_group](./MySQL参数.md#innodb_log_files_in_group)）足够大，甚至和缓冲池一样大，以便能够消除工作负载活动的高峰和低谷。尽管以前redo日志文件过大会导致较长的恢复时间，但现在恢复速度要快得多。
 3. 考虑增加日志缓冲区的大小。更大日志缓冲区使大型事务能够运行，而无需在事务提交之前将日志写入磁盘。因此，如果有更新、插入或删除许多行的事务，则增大日志缓冲区可以节省磁盘I/O。日志缓冲区大小使用[innodb_log_buffer_size](./MySQL参数.md#innodb_log_buffer_size)选项配置，并且在MySQL 8.0中可以动态配置。
 4. 配置[innodb_log_write_ahead_size](./MySQL参数.md#innodb_log_write_ahead_size)选项，避免“read-on-write”。此选项定义redo日志的预写块大小，设置innodb_log_write_ahead_size与操作系统或文件系统缓存块大小匹配，当redo日志的预写块大小与操作系统或文件系统缓存块大小不匹配时，就会发生“写时读”。
 innodb_log_write_ahead_size的有效值为InnoDB日志块大小（2n）的整数倍。最小值为InnoDB日志文件块大小（512）。当指定了最小值时，不会发生预写。最大值等于innodb_page_size的值，如果设置的innodb_log_write_ahead_size大于innodb_page_size，则innodb_log_write_ahead_size会被截断为innodb_page_size。
 innodb_log_write_ahead_size相对于操作系统或文件系统缓存块大小设置过低，会导致写时读。设置过高可能会对日志文件写入的fsync性能有轻微的影响，因为会同时写入多个块。
 5. 从 MySQL 8.0.11开始，引入了专门的日志写线程，用于将redo日志从日志缓冲区写到系统缓冲区，并将系统缓冲区flush到redo日志文件中。以前，单个用户线程负责这些任务。从MySQL 8.0.22开始，可以使用innodb_log_writer_threads变量来启用或禁用日志写线程。专用日志写入线程可以提高高并发系统的性能，但对于低并发系统，禁用专用日志写入线程可以提供更好的性能。
 6. 优化等待redo刷盘的用户线程对自旋延迟的使用。自旋延迟有助于减少latency。在低并发性期间，减少latency的优先级可能更低，并且在这些期间避免使用自旋延迟可能会减少能源消耗。在高并发期间，您可能希望避免在自旋延迟上耗费处理能力，以便将其用于其他工作。以下系统变量允许设置高水位线值和低水位线值，以定义使用自旋延迟的边界。
 - innodb_log_wait_for_flush_spin_hwm：指定用户线程在等待redo刷盘时不再旋转的最大平均日志刷盘时间。缺省值是400微秒。
 - innodb_log_spin_cpu_abs_lwm：指定在等待刷盘时用户线程不再旋转的最小CPU占用量。该值表示为CPU核心使用率的总和。例如，默认值80是单个CPU核心的80%。在具有多核处理器的系统上，值150表示一个CPU核的100%使用率加上第二个CPU核的50%使用率。
 - innodb_log_spin_cpu_pct_hwm：指定在等待刷盘时用户线程不再旋转的最大CPU使用率。该值表示为所有CPU核的总处理能力之和的百分比。缺省值是50%。例如，在具有4个CPU核的服务器上，2个CPU核的100%使用率是总CPU处理能力的50%。
 - innodb_log_spin_cpu_pct_hwm：配置选项尊重处理器亲和性。例如，如果一个服务器有48个核，但是mysqld进程被固定为只有4个CPU核，那么其他44个CPU核将被忽略。

## 优化buffer pool刷盘

# 参考
 * [Redo Log File Configuration](https://dev.mysql.com/doc/refman/8.0/en/innodb-init-startup-configuration.html#innodb-startup-log-file-configuration)
 * [Optimizing InnoDB Redo Logging](https://dev.mysql.com/doc/refman/8.0/en/optimizing-innodb-logging.html)
 * [](https://www.cnblogs.com/yhq1314/p/11319737.html)
 * [](https://www.cnblogs.com/geaozhang/p/7184712.html)
 * [](https://cloud.tencent.com/developer/article/1596411)
