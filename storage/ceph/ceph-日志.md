Ceph 日志
=========

# PGLog
PGLog在代码实现中有3个主要的数据结构来维护：`./src/OSD/osd_types.h:pg_info_t`, `./src/OSD/osd_types.h:pg_log_t`, `./src/OSD/osd_types.h:pg_log_entry_t`。

| op_name       |num| op                                                          |                                     |
| :------------ |:-:| :---------------------------------------------------------- | :---------------------------------- |
| MODIFY        | 1 | some unspecified modification (but not *all* modifications) | 一些未指定的修改（但不是“所有”修改）   |
| CLONE         | 2 | cloned object from head                                     | 从head克隆对象                       |
| DELETE        | 3 | deleted object                                              | 删除对象                             |
| ~~BACKLOG~~   | 4 | event invented by generate_backlog [obsolete]               | generate_backlog事件[已过时]         |
| LOST_REVERT   | 5 | lost new version, revert to an older version.               | 丢失新版本，恢复为旧版本。             |
| LOST_DELETE   | 6 | lost new version, revert to no object (deleted).            | 丢失新版本，恢复为无对象（已删除）      |
| LOST_MARK     | 7 | lost new version, now EIO                                   | 丢失新版本，现在EIO                   |
| PROMOTE       | 8 | promoted object from another tier                           | 来自另一层的提升对象                   |
| CLEAN         | 9 | mark an object clean                                        | 将对象标记为清理                      |
| ERROR         | 10| write that returned an error                                | 写入后返回错误                        |

从结构上可以得知，PGLog里只有对象更新操作相关的内容，没有具体的数据以及偏移大小等，所以后续以PGLog来进行恢复时都是按照整个对象来进行恢复的（默认对象大小是4MB）。

# JOURNAL
FileStore OSDs使用journal有两个原因：速度（speed ）和一致性（consistency）。请注意，自自从Luminous版本，Ceph OSD后端首选和默认的是BlueStore。
 - **Speed:**journal使Ceph OSD Daemon能够快速提交小写操作。Ceph按顺序向journal写入小的随机的i/o，这会让备份文件系统有更多时间合并写入，从而加快突发性工作负载。然而，Ceph OSD守护进程的journal会导致性能尖峰，即在文件系统赶上journal是，会出现短时间的高速写，然后是没有任何写进展的周期。
 - **Consistency:**Ceph OSD守护进程需要一个保证原子复合操作的文件系统接口。Ceph OSD守护进程将操作描述写入日志，并将操作提交给文件系统。这将允许对对象进行原子更新（例如，归置组元数据）。每隔几秒（在filestore max sync interval和filestore min sync interval之间），Ceph OSD守护进程停止写并将journal与文件系统同步，从而允许Ceph OSD守护进程裁剪journal并重用空间。在出现失败时，Ceph OSD守护进程重放上次同步操作之后的journal。

## Ceph OSD守护进程journal设置
 - journal_dio:Enables direct i/o to the journal. Requires `journal block align` set to `true`.
 - journal_aio:Enables using libaio for asynchronous writes to the journal. Requires `journal dio` set to `true`. Version 0.61 and later, `true`. Version 0.60 and earlier, `false`.
 - journal_block_align:Block aligns write operations. Required for `dio` and `aio`.
 - journal_max_write_bytes:The maximum number of bytes the journal will write at any one time.
 - journal_max_write_entries:The maximum number of entries the journal will write at any one time.
 - journal_align_min_size:Align data payloads greater than the specified minimum.
 - journal_zero_on_create:Causes the file store to overwrite the entire journal with `0`’s during `mkfs`.

## FileStore的问题
 - FileStore需要兼容Linux下的各种文件系统（如：EXT4、BtrFS、XFS），理论上每种文件系统都实现了POSIX协议，但事实上每个文件系统都有一点“不那么标准”的地方。Ceph的实现非常注重可靠性，因而需要为每种文件系统引入不同的walkaround或者hack，例如rename不幂等性。POSIX提供了非常强大的功能，但大部分Ceph并不真正需要，反而成了性能上的累赘；某些功能实现对Ceph并不友好（例如：对目录遍历顺序的要求）。
 - FileStore为了保证覆写中途断电能够恢复，以及为了实现单OSD内的事务支持，首先把（数据或元数据的）修改写入journal，journal落盘后再将数据或元数据落盘。这种WAL机制是数据库和文件系统标准的保证ACID的方法，但用在Ceph这里，带来了问题：
    1. 双写问题，这意味着Ceph牺牲了一半的磁盘吞吐量。
    2. Journaling of Journal问题，FileStore有journal机制，而Linux文件系统自身也有日志机制，实际上存在双重日志。

## BlueStore的改进
 - Ceph并不需要POSIX文件系统。抛弃它，实现一个尽量简单的文件系统，专门给RocksDB使用。这个文件系统叫作BlueFS。数据不需要文件系统，直接存储在裸块设备上即可，在块设备上需要的其实是一个空间分配器（Allocator）。
 - 元数据存储在RocksDB中，用KeyValue的方式正合适。对于新型的LSM-Tree类存储（如：RocksDB、LevelDB），由于数据本身就按照日志形式组织，所以不再需要单独记日志。

# 参考
 * [解析Ceph: 恢复与数据一致性](https://cloud.tencent.com/developer/article/1529095)
 * [LOG BASED PG](https://docs.ceph.com/en/latest/dev/osd_internals/log_based_pg/)
 * [JOURNAL CONFIG REFERENCE](https://docs.ceph.com/en/latest/rados/configuration/journal-ref/)
 * [Ceph后端存储引擎BlueStore](https://cloud-atlas.readthedocs.io/zh_CN/latest/ceph/bluestore.html)
 * [Ceph的BlueStore总体介绍](https://blog.csdn.net/u010487568/article/details/79572390)
 * [Ceph BlueStore 和双写问题](https://cloud.tencent.com/developer/news/45599)
 * [BLUESTORE INTERNALS](https://docs.ceph.com/en/latest/dev/bluestore/)
