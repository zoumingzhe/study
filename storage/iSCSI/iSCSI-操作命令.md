iSCSI 操作命令
=============

# 查看版本号
 - `iscsid -v`
 - `targetcli -v`

# initiator
`yum -y install iscsi-initiator-utils`

## InitiatorName
### iscsi-iname
`iscsi-iname [-p <iqn>]`

每次调用时生成一个唯一的iSCSI节点名称。

### /etc/iscsi/initiatorname.iscsi
```shell
# 生成iqn
echo "InitiatorName=`iscsi-iname`" > /etc/iscsi/initiatorname.iscsi
# 定义别名
echo "InitiatorAlias=<alias>" >> /etc/iscsi/initiatorname.iscsi
```

# [targetcli](https://github.com/Datera/targetcli)
基本语法：`[TARGET_PATH] COMMAND_NAME [OPTIONS]`

The TARGET_PATH indicates the path to run the command from.
If ommited, the command will be run from your current path.

The OPTIONS depend on the command. Please use help
COMMAND to get more information.

*一个host只能同时执行一个targetcli。*

## COMMANDS
  - `bookmarks action [bookmark]`
  - `cd [path]`：进入目录
  - `exit`：退出targetcli命令行
  - `get [group] [parameter...]`
  - `help [topic]`
  - `info`
  - `ls [path] [depth]`：显示当前目录的详细信息
  - `pwd`：显示当前目录
  - `refresh`：刷新
  - `set [group] [parameter=value...]`
  - `status`：显示当前目录的摘要信息

## get 命令
`get [<group>] [<parameter> ...]`

列出给定组中的一个或多个配置参数的值。

若指定了一个组，则列出该组中的所有可用参数。

若无指定组，则列出所有可用的组。

例如：`get global auto_add_mapped_luns`

## set 命令
`set [<group>] [<parameter>=<value> ...]`

设置给定组中的一个或多个配置参数的值。

例如：`set global auto_add_mapped_luns=true`

## 后端存储 /backstores
### /backstores/block
`create <name> <device> [<readonly>] [<wwn>]`
 - *name*：后端存储名。
 - *device*：块设备。

### /backstores/fileio
`create <name> <file_or_device> [<size>] [<generate_wwn>] [<buffered>]`
 - *name*：后端存储名。
 - *file_or_device*：指定镜像文件或块设备。
 - *size*：若为镜像文件，指定镜像文件空间大小。
    + k, K, kB, KB for kB (kilobytes)
    + m, M, mB, MB for MB (megabytes)
    + g, G, gB, GB for GB (gigabytes)
    + t, T, tB, TB for TB (terabytes)
 - *generate_wwn*：默认自动生成WWN号。
 - *buffered*：是否为设备启用写缓存，默认禁止写缓存（即同步I/O）。

### /backstores/pscsi
使用物理的SCSI磁盘设备做为后端存储，iSCSI将直接使用SCSI驱动操纵物理设备，而不使用SCSI模拟器。

`create <name> <device>`
 - *name*：后端存储名。
 - *device*：SCSI设备的完整路径名或SCSI设备ID。如果使用的是路径名，Datera建议使用`/dev/disk/by-id`层次结构，以确保在重新引导或修改底层物理SCSI系统时保持命名的一致。

### /backstores/ramdisk
`create <name> <size>`
 - *name*：后端存储名。
 - *size*：空间大小。
    + k, K, kB, KB for kB (kilobytes)
    + m, M, mB, MB for MB (megabytes)
    + g, G, gB, GB for GB (gigabytes)
    + t, T, tB, TB for TB (terabytes)

### delete
`delete <name>`
如果有LUN使用这个后端存储，它们也将被删除。

## /iscsi
### luns
`create <storage_object> [<lun>] [<add_mapped_luns>]`
 - *storage_object*：指定要关联的后端存储设备对象的**绝对路径**。
 - *lun*：指定LUN编号，默认从0开始自动分配。
 - *add_mapped_luns:true*：自动为新的LUN关联可用的ACL。若不指定，默认使用全局的*auto_add_mapped_luns*值。

`delete <lun>`
 - <lun> must be an integer matching an existing LUN.
Delete the specified LUN from the TPG.

### acls
`create <wwn> [<add_mapped_lun>]`

`delete <wwn>`
 - <wwn> must be a valid existing WWN.
Delete the Node ACL with the specified wwn.

`create <mapped_lun> <tpg_lun_iscsi> [<write_protect>]`
 - *mapped_lun*：启动程序中的映射LUN。
 - *tpg_lun_iscsi*：对应的TPG LUN（仅iSCSI）。
 - *write_protect*：一个可选的布尔参数，指定Initiator是否具有对映射LUN的写访问权。

`delete <mapped_lun>`
 - <mapped_lun> must be a integer matching an existing Mapped LUN.
Delete the specified Mapped LUN from the TPG.

### 创建 Portal
`create [<ip_address>] [<ip_port>]`
Create an iSCSI Network Portal with specified ip_address and ip_port.

`delete <ip_address> <ip_port>`
Delete the iSCSI Network Portal with the ip_address and ip_port.

# 参考
 * [targetcli](http://linux-iscsi.org/wiki/Targetcli)
 * [GETTING STARTED WITH ISCSI](https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/8/html/managing_storage_devices/getting-started-with-iscsi_managing-storage-devices)
 * [targetcli](http://linux.51yip.com/search/targetcli)
 * [iSCSI 原理和基础使用](https://www.cnblogs.com/wn1m/p/10700591.html)
