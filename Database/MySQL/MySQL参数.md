# MySQL参数

## Server System Variables
### basedir
指定MySQL的安装目录。

### datadir
指定MySQL的数据目录。相对路径相对于当前目录进行解析，如果希望服务自动启动（即无法预知当前目录的上下文）最好将datadir指定为绝对路径。

### tmpdir
指定用于创建临时文件的目录。如果默认的/tmp目录所在的分区太小，无法容纳临时表，那么它可能很有用。可以将此变量设置为以循环方式使用的多个路径的列表，在Unix上路径应以冒号字符（:）分隔，在Windows上路径应以分号字符（;）分隔。

tmpdir可以是非永久性位置，例如基于内存的文件系统上的目录或主机重启时清除的目录。如果MySQL服务作为一个replica，而tmpdir使用的是非永久位置，请考虑使用slave_load_tmpdir变量为replica设置一个不同的临时目录。对于replica，用于复制LOAD DATA语句的临时文件存储在tmpdir目录中，因此tmpdir使用永久位置，临时文件可以在计算机重新启动后继续存在，如果临时文件被删除，则在重新启动后继续复制。

## Server Status Variables
### Binlog_cache_disk_use
指定使用binlog缓存但超过[binlog_cache_size](#binlog_cache_size)值并使用临时文件存储事务中语句的事务数。

写入磁盘的非事务语句数由[Binlog_stmt_cache_disk_use](#Binlog_stmt_cache_disk_use)单独跟踪。

### Binlog_cache_use
指定使用binlog缓存的事务数。

### Binlog_stmt_cache_disk_use
指定指定使用binlog语句缓存但超过[binlog_stmt_cache_size](#binlog_stmt_cache_size)值并使用临时文件存储的非事务语句数。

### Binlog_stmt_cache_use
指定使用binlog语句缓存的非事务语句数。

## InnoDB Startup Options and System Variables
### innodb_buffer_pool_filename

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

### innodb_data_file_path
指定InnoDB系统表空间数据文件的名称、大小和属性。如果没有为innodb_data_file_path指定值，默认创建一个自动扩展的数据文件，略大于12MB，命名为ibdata1。

数据文件规范的完整语法包括文件名、文件大小、autoextend属性和max属性：
```
file_name:file_size[:autoextend[:max:max_file_size]]
```

文件大小以千字节、兆字节或千兆字节（通过在大小值后面附加K、M或G）指定。如果以KB为单位指定数据文件大小，请以1024的整数倍指定，否则KB值将四舍五入到最近的MB边界。文件大小的总和至少要略大于12MB。

### innodb_data_home_dir
指定InnoDB系统表空间数据文件路径的公共部分目录，默认值[MySQL的数据目录](#datadir)。该设置与[innodb_data_file_path](#innodb_data_file_path)串联，除非该设置用绝对路径定义。

为innodb_data_home_dir指定值时需要一个尾部斜杠。例如：
```
[mysqld]
innodb_data_home_dir = /path/to/myibdata/
```

此设置不影响每个表的文件表空间的位置。

### innodb_dedicated_server （从MySQL 8.0开始）
当启用innodb_dedicated_server时，InnoDB会自动配置以下变量：
 - innodb_buffer_pool_size
 - innodb_log_file_size
 - innodb_log_files_in_group （从MySQL 8.0.14开始）
 - innodb_flush_method

只有当MySQL实例位于可以使用所有可用系统资源的专用服务器上时，才考虑启用innodb_dedicated_server。如果MySQL实例与其他应用程序共享系统资源，则不建议启用innodb_dedicated_server。

### innodb_directories
指定在启动时扫描表空间文件的目录。此选项用于在服务离线时将表空间文件移动或恢复到新位置。它还用于指定使用绝对路径创建或位于数据目录之外的表空间文件的目录。

崩溃恢复期间的表空间发现依赖于innodb_directories来标识重做日志中引用的表空间。

默认值NULL，但是当InnoDB创建一个启动时要扫描的目录列表时，innodb_data_home_dir、innodb_undo_directory和datadir定义的目录总是附加到innodb_directories之后。无论是否显式指定了innodb_directories，这些目录都会被追加。

innodb_directories可以在启动命令或MySQL选项文件中指定。引号包围参数值是因为有些命令解释器将分号（;）解释为特殊字符。（例如，Unix shell将其视为命令终止符。）

Startup command:
```
mysqld --innodb-directories="directory_path_1;directory_path_2"
```

MySQL option file:
```
[mysqld]
innodb_directories="directory_path_1;directory_path_2"
```

通配符表达式不能用于指定目录。

innodb_directories扫描也会遍历指定目录的子目录，并丢弃待扫描目录列表中的重复目录和子目录。

### innodb_doublewrite_dir
指定doublewrite文件目录。如果不指定innodb_doublewrite_dir，则在[innodb_data_home_dir](#innodb_data_home_dir)目录下创建doublewrite文件;如果未指定innodb_data_home_dir，则默认为[数据目录](#datadir)。

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

### innodb_redo_log_archive_dirs

### innodb_temp_data_file_path

### innodb_temp_tablespaces_dir

### innodb_tmpdir
指定重建表的在线ALTER TABLE操作期间创建的临时排序文件的备用目录。

重新生成表的在线ALTER TABLE操作还会在与原始表相同的目录中创建一个中间表文件。innodb_tmpdir选项不适用于中间表文件。

有效值是除[MySQL数据目录](#datadir)外的任何目录，如果指定为NULL（默认值），则创建MySQL临时目录（Unix上为$TMPDIR, Windows上为%TEMP%，或者由--tmpdir选项指定的目录）。如果指定了目录，则只在使用SET语句配置innodb_tmpdir时检查目录是否存在和权限。如果在目录字符串中提供符号链接，则解析符号链接并将其存储为绝对路径，路径不能超过512字节。如果将innodb_tmpdir设置为无效目录，在线ALTER TABLE操作会报错。innodb_tmpdir覆盖MySQL的tmpdir设置，但仅用于在线ALTER TABLE操作。

配置innodb_tmpdir需要FILE权限。

引入innodb_tmpdir选项是为了避免tmpfs文件系统上的临时文件目录溢出。这种溢出可能是在重新构建表的联机ALTER TABLE操作期间创建的大型临时排序文件造成的。

在replication环境中，仅当所有服务都具有相同的操作系统环境时，才考虑复制innodb_tmpdir。否则，复制innodb_tmpdir可能导致在运行重建表的在线ALTER TABLE操作时复制失败。如果服务器运行环境不同，建议在每台服务器上分别配置innodb_tmpdir。

### innodb_undo_directory

## Replica Server Options and Variables

## Binary Logging Options and Variables
### binlog_cache_size
指定一个事务binlog的内存缓冲区大小，该值必须是4096的整数倍。

当启用binlog记录时（[log_bin](#log_bin)设置为ON），如果服务支持任何事务性存储引擎，则会为每个客户端分配一个binlog缓存。如果事务的数据大小超过内存缓冲区的空间，则多余的数据将存储在临时文件中。当binlog加密处于活动状态时，内存缓冲区不会加密，但是（从MySQL 8.0.17开始）用于保存binlog缓存的任何临时文件都会加密。在每个事务被提交之后，通过清理内存缓冲区和清空临时文件（如果使用）来重置binlog缓存。

如果经常使用大型事务，则可以通过增加缓存大小来减少或消除对临时文件的写操作以获得更好的性能，[Binlog_cache_use](#Binlog_cache_use)和[Binlog_cache_disk_use](#Binlog_cache_disk_use)状态变量可用于调整此变量的大小。

binlog_cache_size只设置事务缓存的大小；语句缓存的大小由binlog_stmt_cache_size指定。

### binlog_checksum
指定是否启用为每个源为binlog的事件写入校验和。binlog_checksum支持的值为NONE（禁用校验和）和CRC32，默认值为CRC32。当binlog_checksum被禁用（值为NONE）时，通过写入和检查每个事件的事件长度（而不是校验和）来验证它是否只将完整的事件写入binlog。

将source的此变量设置为replica无法识别的值将导致replica将其自己的binlog_checksum值设置为NONE，并出现错误停止复制。如果需要考虑与旧副本的向后兼容性，则可能需要将该值显式设置为NONE。

在MySQL8.0.20之前（包括MySQL8.0.20），Group Replication不能使用校验和，并且不支持它们在binlog中的存在，因此在将服务实例配置为group member时，必须将binlog_checksum设置为NONE。在MySQL 8.0.21开始，Group Replication支持校验和，因此group member可以使用默认设置。

更改binlog_checksum的值会导致binlog旋转，因为必须为整个binlog文件写入校验和，而不能仅为其中一个文件的一部分写入校验和。不能在事务中更改binlog_checksum的值。

当使用[binlog_transaction_compression](#binlog_transaction_compression)启用binlog事务压缩时，不会为压缩事务负载中的单个事件写入校验和，而是为GTID事件写一个校验和，并且为压缩事务负载事件写一个校验和。

### binlog_encryption
指定是否启用binlog文件和中继日志文件的加密，默认为OFF。ON为binlog文件和中继日志文件设置加密。不需要在服务器上启用binlog来启用加密，因此可以在没有binlog的replica上加密中继日志文件。要使用加密，必须安装并配置一个keyring插件以提供MySQL服务器的keyring服务，任何支持的keyring插件都可以用来存储binlog加密密钥。

在首次启动启用binlog加密的服务时，将在初始化binlog和中继日志之前生成新的binlog加密密钥。此密钥用于加密每个binlog文件（如果启用了binlog记录）和中继日志文件（如果存在replication channels）的文件密码，并且使用从文件密码生成的其他密钥加密文件中的数据。所有通道的中继日志文件都将被加密，包括Group Replication的applier channels和激活加密后创建的new channels。binlog索引文件和中继日志索引文件从不加密。

如果在服务运行时激活加密，则此时会生成一个新的binlog加密密钥。例外情况是，如果加密以前在服务上被激活过然后被禁用，在这种情况下以前使用的binlog加密密钥将被再次使用。binlog文件和中继日志文件将立即轮换，新文件和所有后续binlog文件和中继日志文件的文件密码将使用此binlog加密密钥进行加密。服务上仍然存在的现有binlog文件和中继日志文件不会自动加密，但如果不再需要它们，可以清除它们。

如果将binlog_encryption更改为OFF来停用加密，binlog文件和中继日志文件将立即旋转，并且所有后续日志记录都将取消加密。以前加密的文件不会自动解密，但服务器仍然能够读取它们。在服务运行时激活或停用加密需要BINLOG_ENCRYPTION_ADMIN特权（或不推荐使用的SUPER特权）。Group Replication的applier channels不包括在中继日志轮换请求中，因此这些通道正常使用中的未加密日志记录在轮换日志之前不会启动。

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
 * [5.1.8 Server System Variables](https://dev.mysql.com/doc/refman/8.0/en/server-system-variables.html)
 * [5.1.10 Server Status Variables](https://dev.mysql.com/doc/refman/8.0/en/server-status-variables.html)
 * [15.14 InnoDB Startup Options and System Variables](https://dev.mysql.com/doc/refman/8.0/en/innodb-parameters.html)
 * [17.1.6.3 Replica Server Options and Variables](https://dev.mysql.com/doc/refman/8.0/en/replication-options-replica.html)
 * [17.1.6.4 Binary Logging Options and Variables](https://dev.mysql.com/doc/refman/8.0/en/replication-options-binary-log.html)

