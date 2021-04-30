# MySQL参数

## InnoDB Startup Options and System Variables
### innodb_buffer_pool_size
缓冲池（buffer pool）的字节大小，InnoDB缓存表和索引数据的内存区域。最大值取决于CPU架构，32位系统上的最大值为4294967295（2^32 - 1），64位系统上的最大值为18446744073709551615（2^64 - 1）。

当缓冲池大小大于1GB时，将innodb_buffer_pool_instances设置为大于1的值可以提高繁忙服务器上的可伸缩性。

更大的缓冲池需要更少的磁盘I/O来多次访问同一个表数据。在专用数据库服务器上，可以将缓冲池大小设置为计算机物理内存大小的80%。在配置缓冲池大小时，请注意以下潜在问题，并准备在必要时缩小缓冲池的大小。

对物理内存的竞争可能导致操作系统中的分页。

InnoDB为buffers和控制结构保留了额外的内存，因此分配的总空间大约比指定的缓冲池大小大10%。

缓冲池的地址空间必须是连续的，这在具有在特定地址加载DLL的Windows系统上可能是一个问题。

初始化缓冲池的时间大致与其大小成正比。在具有大型缓冲池的实例上，初始化时间可能很长。为了缩短初始化周期，可以在MySQL服务关闭时保存缓冲池状态，并在MySQL服务启动时恢复它。

当您增加或减少缓冲池大小时，操作将以chunks的形式执行。chunk大小由innodb_buffer_pool_chunk_size变量定义。

缓冲池大小必须始终等于或是innodb_buffer_pool_chunk_size * innodb_buffer_pool_instances的整数倍。如果将缓冲池大小更改为不等于innodb_buffer_pool_chunk_size * innodb_buffer_pool_instances或其整数倍，则缓冲池大小将自动调整为等于innodb_buffer_pool_chunk_size * innodb_buffer_pool_instances或其整数倍。

innodb_buffer_pool_size可以动态设置，允许在不重新启动MySQL服务的情况下调整缓冲池大小。innodb_buffer_pool_resize_status变量报告联机缓冲池大小调整操作的状态。

如果启用innodb_dedicated_server ，则如果未明确定义innodb_buffer_pool_size值，则会自动配置该值。

### innodb_change_buffer_max_size
InnoDB写缓冲（change buffer）的最大大小占缓冲池（buffer pool）总大小的百分比。对于具有大量insert、update和delete等写操作的服务可以增加该值，对于大部分为读操作的服务可以减少该值。

### innodb_change_buffering
InnoDB是否执行change buffering，这是一种将写入操作延迟到二级索引的优化，以便可以按顺序执行I/O操作。

下表描述了允许值，数值也可以用数字表示。
| Value   | Numeric Value | Description                             |
| :------ | :-----------: | :-------------------------------------- |
| none    |       0       | 不缓冲任何操作                           |
| inserts |       1       | 缓冲插入操作                             |
| deletes |       2       | 缓冲标记删除操作                         |
| changes |       3       | 缓冲插入和标记删除操作                    |
| purges  |       4       | 缓冲后台物理删除操作                      |
| all     |       5       | 默认值，缓冲插入、标记删除和物理删除操作    |

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

### innodb_old_blocks_pct
用于指定old block sublist占InnoDB缓冲池的近似百分比，通常与innodb_old_blocks_time结合使用。

### innodb_old_blocks_time
指定插入到old sublist的块在第一次访问后必须在old sublist停留多少毫秒，然后才能移动到new sublist，通常与innodb_old_blocks_pct结合使用。

如果该值为0，则插入到old sublist中的块在第一次被访问时会立即移动到new sublist，无论插入后多久进行访问。如果该值大于0，则块将保留在old sublist中，直到在第一次访问之后至少innodb_old_blocks_time毫秒再次发生访问为止。例如，值1000会导致块在第一次访问后在old sublist中停留1秒，然后才能在被再次访问时移动到new sublist。

非零值可以防止缓冲池被只在短时间内引用的数据填充（例如在全表扫描期间），增加此值可以提供更多的保护，以防全表扫描干扰缓存在缓冲池中的数据。

## Binary Logging Options and Variables
### binlog_expire_logs_seconds
指定以秒为单位的binlog过期时间并自动删除过期的binlog文件。binlog文件删除可能发生在startup和binlog刷盘时。默认的binlog过期时间是2592000秒，等于30天（30*24*60*60秒）。

如果在启动时binlog_expire_logs_seconds和expire_logs_days都没有设置值，则默认值适用；如果在启动时为其中一个变量设置了非零值，则该值将用作binlog过期期限；如果在启动时为这两个变量都设置了非零值，则binlog_expire_logs_seconds的值将用作binlog过期期限，expire_logs_days的值将被忽略并显示警告消息。

要禁用binlog的自动清除，请将binlog_expire_logs_seconds显式指定为0，而不要为expire_logs_days指定值。为了与早期版本兼容，如果将expire_logs_days显式指定为0，而没有为binlog_expire_logs_seconds指定值，则也将禁用自动清除。在这种情况下，binlog_expire_logs_seconds的默认值不适用。

要手动删除binlog文件，请使用PURGE BINARY LOGS语句。

### expire_logs_days
指定binlog的过期时间并自动删除过期的binlog文件。在MySQL 8.0版本expire_logs_days已弃用，使用[binlog_expire_logs_seconds](#binlog_expire_logs_seconds)代替，以秒为单位设置binlog过期时间。如果未为任一系统变量设置值，则默认的过期期限为30天。binlog文件删除可能发生在startup和binlog刷盘时。

如果同时指定了binlog_expire_logs_seconds，则expire_logs_days指定的任何非零值都将被忽略，而binlog_expire_logs_seconds的值将用作binlog过期时间，在这种情况下会发出警告消息。expire_logs_days的非零值仅在未指定binlog_expire_logs_seconds或将其指定为0时才用作binlog过期期限。

要禁用binlog的自动清除，请将binlog_expire_logs_seconds显式指定为0，而不要为expire_logs_days指定值。为了与早期版本兼容，如果将expire_logs_days显式指定为0，而没有为binlog_expire_logs_seconds指定值，则也将禁用自动清除。在这种情况下，binlog_expire_logs_seconds的默认值不适用。

要手动删除binlog文件，请使用PURGE BINARY LOGS语句。

### log_bin
用于指定MySQL服务是否记录binlog，可以是启用（on）或禁用（OFF）。启用binlog记录后，服务器将所有更改数据的语句记录到binlog中，binlog用于备份和复制。ON表示binlog可用，OFF表示binlog未被使用。--log bin选项可用于指定binlog的基本名称和位置。

在早期的MySQL版本中，默认情况下禁用binlog记录，如果指定--log bin选项，则启用binlog记录。在MySQL 8.0中，binlog记录在默认情况下是启用的，logbin系统变量设置为ON，无论您是否指定--logbin选项。例外情况是，如果您使用mysqld手动初始化数据目录，方法是使用--initialize或--initialize unsecure选项调用它，而binlog记录在默认情况下是禁用的。在这种情况下，可以通过指定--log bin选项来启用binlog记录。

如果在启动时指定--skip log bin或--disable log bin选项，则禁用binlog记录，并将log_bin系统变量设置为OFF。如果指定了这些选项中的任何一个并且还指定了--log bin，则后面指定的选项优先。


# 参考
1.[15.14 InnoDB Startup Options and System Variables](https://dev.mysql.com/doc/refman/8.0/en/innodb-parameters.html)

2.[17.1.6.4 Binary Logging Options and Variables](https://dev.mysql.com/doc/refman/8.0/en/replication-options-binary-log.html)

