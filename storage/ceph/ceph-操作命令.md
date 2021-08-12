ceph 操作命令
=============

#  查看集群状态
## `ceph -s`
查看集群状态
## `ceph health [detail]`
查看集群健康状态
## `ceph df`
查看集群使用情况

# ceph mon
## `ceph mon stat`
查看集群Monitor状态
## `ceph mon dump`
查看集群Monitor的monmap
## `ceph quorum_status`


# ceph osd
## `ceph osd stat`
查看集群OSD状态
## `ceph osd tree`
查看集群OSD分布
## `ceph osd dump`
等同于：
 - `ceph osd getmap -o osdmap`
 - `osdmaptool --print osdmap`
## `ceph osd df`
## `ceph osd down <osd.n>`
## `ceph osd up <osd.n>`
## `ceph osd out <osd.n>`
## `ceph osd in <osd.n>`

# ceph osd pool
## `ceph osd lspools`
查看POOL列表
## `ceph osd pool ls [detail]`
查看集群POOL列表
## `ceph osd pool get <poolname> <key>`
查看POOL变量
## `ceph osd pool set <poolname> <key> <value>`
设置POOL变量
## `ceph osd pool create <poolname> <int[0-]> {<int[0-]>} {replicated|erasure} {<erasure_code_profile>} {<rule>} {<int>}`
创建POOL
## `ceph osd pool rename <current-poolname> <new-poolname>`
重命名POOL
## `ceph osd pool delete <poolname> <poolname> --yes-i-really-really-mean-it`
删除POOL
## `ceph osd pool mksnap <poolname> <snap>`
创建POOL快照
## `ceph osd pool rmsnap <poolname> <snap>`
删除POOL快照

# ceph pg
## `ceph pg dump [--format json]`
## `ceph pg dump_stuck <>`

# ceph auth
## `ceph auth list`


# rados
## POOL
 - `rados df`
 - `rados lspools`
 - `rados -p <poolname> ls`
 - `rados mkpool <pool-name> [123[ 4]]`
 - `rados cppool <pool-name> <dest-pool>`
 - `rados rmpool <pool-name> [<pool-name> --yes-i-really-really-mean-it]`
 - `rados purge <pool-name> --yes-i-really-really-mean-it`
## OBJECT
### object
 - `rados -p <poolname> stat <obj-name>`
 - `rados -p <poolname> get <obj-name> [outfile]`
 - `rados -p <poolname> put <obj-name> [infile] [--offset offset]`
 - `rados -p <poolname> append <obj-name> [infile]`
 - `rados -p <poolname> truncate <obj-name> length`
 - `rados -p <poolname> create <obj-name>`
 - `rados -p <poolname> rm <obj-name> ...[--force-full]`
 - `rados -p <poolname> cp <obj-name> [target-obj]`
### xattr
 - `rados -p <poolname> listxattr <obj-name>`
 - `rados -p <poolname> getxattr <obj-name> attr`
 - `rados -p <poolname> setxattr <obj-name> attr val`
 - `rados -p <poolname> rmxattr <obj-name> attr`
### omap
 - `rados -p <poolname> listomapkeys <obj-name>`
 - `rados -p <poolname> listomapvals <obj-name> <key>`
 - `rados -p <poolname> getomapval <obj-name> <key>`
 - `rados -p <poolname> setomapval <obj-name> <key> <val>`
 - `rados -p <poolname> rmomapkey <obj-name> <key>`
 - `rados -p <poolname> getomapheader <obj-name> [file]`
 - `rados -p <poolname> setomapheader <obj-name> <val>`
### Watch/Notify
 - `rados -p <poolname> listwatchers <obj-name>`
 - `rados -p <poolname> watch <obj-name>`
 - `rados -p <poolname> notify <obj-name> <message>`
## IMPORT AND EXPORT
 - `rados -p <poolname> export [filename]`
 - `rados -p <poolname> import [--dry-run] [--no-overwrite] < filename | - >`

# 参考
 * []()
