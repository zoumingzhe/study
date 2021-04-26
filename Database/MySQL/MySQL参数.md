# MySQL参数

## InnoDB Startup Options and System Variables
### innodb_dedicated_server （从MySQL 8.0开始）
当启用innodb_dedicated_server时，InnoDB会自动配置以下变量：
 - innodb_buffer_pool_size
 - innodb_log_file_size
 - innodb_log_files_in_group （从MySQL 8.0.14开始）
 - innodb_flush_method

只有当MySQL实例位于可以使用所有可用系统资源的专用服务器上时，才考虑启用innodb_dedicated_server。如果MySQL实例与其他应用程序共享系统资源，则不建议启用innodb_dedicated_server。

### innodb_flush_neighbors
指定是否在刷盘InnoDB buffer pool中的页同时刷盘同一extent内的其他脏页。
 - 设置为0，禁用innodb_flush_neighbors。相同extent内的脏页不会被刷盘。
 - 设置为1，将刷盘相同extent内的连续脏页。
 - 设置为2，将刷盘相同extent内的所有脏页。

对于存储在传统HDD上的表数据，磁盘寻道时间在I/O开销中占比很大，相比于在不同时间刷盘单个页，在一次操作中刷盘相邻页可减少I/O平均时延，提升磁盘带宽。对于存储在SSD上的表数据，寻道时间不是一个重要因素，可以将此选项设置为0，可以降低总擦写次数。对于存储在分布式存储（例如ceph）上的表数据，需要具体。

### innodb_io_capacity
可用于InnoDB后台任务的每秒I/O次数（IOPS），例如buffer pool脏页刷盘和change buffer数据合并。

### innodb_io_capacity_max
如果刷盘出现滞后，则InnoDB每秒I/O次数（IOPS）可以高于innodb_io_capacity，innodb_io_capacity_max定义了在这种情况下InnoDB后台任务的每秒最大I/O次数（IOPS）。

### innodb_log_file_size
日志组中每个日志文件的字节大小，默认值为48MB。日志组的总文件大小（innodb_log_file_size * innodb_log_files_in_group）不能超过最大值512GB。innodb_log_file_size最小值为4MB。

通常，日志组的总文件大小应该足够大，以便服务器能够平滑高峰和低谷的工作负载活动，这意味着重做日志空间需要足够处理一个小时以上的日志写。日志文件越大，checkpoint时buffer pool中所需刷盘的脏页越少，从而节省磁盘I/O，但崩溃恢复速度也越慢。

如果启用了innodb_dedicated_server，则如果没有明确定义，则会自动配置innodb_log_file_size值。

### innodb_log_files_in_group
日志组中的日志文件数，默认值为2。InnoDB以循环方式写入重做日志文件。重做日志文件的目录由innodb_log_group_home_dir指定。日志组的总文件大小（innodb_log_file_size * innodb_log_files_in_group）。

需要注意的是innodb_log_files_in_group是静态的变量，更改时需删除已存在的日志文件并重新启动，否则MySQL启动报错。例如将innodb_log_files_in_group从2修改为3，需要先正常停止MySQL服务，删除已存在的ib_logfile0和ib_logfile1文件，然后重启MySQL服务，否则报错。

### innodb_log_group_home_dir
InnoDB重做日志文件的目录路径，其编号由innodb_log_files_in_group指定。如果未指定任何InnoDB日志变量，那么默认在MySQL数据目录中创建两个名为ib_logfile0和ib_logfile1的日志文件。

### innodb_max_dirty_pages_pct
InnoDB尝试将buffer pool中的脏页刷盘，以使脏页在buffer pool中的百分比不超过innodb_max_dirty_pages_pct，它本身并不会不影响刷盘速度。

# 参考
1.[15.14 InnoDB Startup Options and System Variables](https://dev.mysql.com/doc/refman/8.0/en/innodb-parameters.html)
