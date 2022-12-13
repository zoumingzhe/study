dmsetup
=======

low level logical volume management

`dmsetup`管理`device-mapper`设备的逻辑设备。

设备是通过加载一个表来创建的，该表为逻辑设备中的每个扇区（512字节）指定一个目标。

`dmsetup`的第一个参数是一个命令；第二个参数是逻辑设备名称或`uuid`。

以`devmap_name`的形式调用`dmsetup`工具（它不是正态分布的，仅出于历史原因支持）等效于`dmsetup info -c --noheadings -j major -m minor`。



# COMMANDS
## create
```shell
dmsetup create device_name [-n|--notable|--table table|
                table_file] [--readahead [+]sectors|auto|none]
                [-u|--uuid uuid] [--addnodeoncreate|--addnodeonresume]
```

创建具有给定名称的设备。

如果提供了`table`或`table_file`，则加载该表并使其生效。否则，若未使用`--notable`选项，则从标准输入读取`table`。

在后续的`dmsetup`命令中，可以使用可选的`uuid`代替`device_name`。如果成功，设备将出现在表中，对于实时设备，将创建节点`/dev/mapper/device_name`。

有关表格格式的更多信息，请参见下文。

```shell
dmsetup create --concise [concise_device_specification]
```

从简洁的设备规范创建一个或多个设备。

每个设备都由逗号分隔的列表指定：`name`、`uuid`、`minor`、`flags`、逗号分隔的表行。

标志默认为读写（rw）或只读（ro）。`uuid`、`minor`和`flags`是可选的，因此这些字段可能为空。

分号分隔不同设备的规格。

使用反斜杠转义以下字符，例如`name`或`table`中的逗号或分号。

另请参阅下面的简洁格式。


## table
```shell
dmsetup deps [-o options] [device_name...]
```

## targets
```shell
dmsetup targets
```

显示当前已加载`target`的名称和版本。



# table
`device-mapper`设备由一个表定义，该表指定如何使用受支持的设备表来映射`device-mapper`设备的每个逻辑扇区范围。

`device-mapper`设备的表由以下形式的行列表构成：

```shell
start length mapping [mapping_parameters...]
```

每行的前两个参数是段起始块和段的长度；下一个参数是映射的`target`；其余参数由`target`的参数组成。

表中的第一行，`start`参数必须等于0；上一行的`start`+`length`参数必须等于下一行的`start`参数。

在映射表的一行中指定了哪些`mapping_parameters`参数取决于该行中指定了哪种`mapping`类型。

`device-mapper`设备的大小始终以扇区（512字节）指定。

当`mapping_parameters`参数中指定一个设备时，它可以引用文件系统中的设备名称（例如`/dev/hda`）或`major:minor`格式。首选`major:minor`格式，因为它避免了路径名查找。

下面显示了一个`device-mapper`设备映射表的示例，在此表中有四个`linear`目标：

```shell
0 35258368 linear 8:48 65920
35258368 35258368 linear 8:32 65920
70516736 17694720 linear 8:16 17694976
88211456 17694720 linear 8:16 256
```

## linear target
将连续范围的扇区映射到另一个块设备上。
```shell
start length linear device offset
```
 - start
    虚拟设备中的起始块。
 - length
    该段的长度。
 - device
    块设备，文件系统中的设备名称或`major:minor`格式。
 - offset
    块设备上的起始偏移量。

## striped target
支持跨物理设备的条带化。
```shell
start length striped #stripes chunk_size device1 offset1 ... deviceN offsetN
```
 - start
    虚拟设备中的起始块。
 - length
    该段的长度。
 - #stripes
    虚拟设备的条带数。
 - chunk_size
    （在切换至下一个条带前写入）每个条带的扇区数；必须是2的幂，至少应为内核page大小。
 - device
    块设备，文件系统中的设备名称或`major:minor`格式。
 - offset
    块设备上的起始偏移量。

## mirror target
支持镜像逻辑设备的映射。
```shell
start length mirror log_type #logargs logarg1 ... logargN #devs device1 offset1 ... deviceN offsetN
```
 - start
    虚拟设备中的起始块。
 - length
    该段的长度。
 - log_type
    可能的日志类型及其参数如下：
    + core
        镜像是本地的，镜像日志保存在核心内存中。此日志类型采用1-3个参数：
        ```shell
        regionsize [[no]sync] [block_on_error]
        ```
    + disk
        镜像是本地的，镜像日志保存在磁盘上。此日志类型需要2-4个参数：
        ```shell
        logdevice regionsize [[no]sync] [block_on_error]
        ```
    + clustered_core
        镜像是集群的，镜像日志保存在核心内存中。此日志类型需要2-4个参数：
        ```shell
        regionsize UUID [[no]sync] [block_on_error]
        ```
    + clustered_disk
        镜像是集群的，镜像日志保存在磁盘上。此日志类型需要3-5个参数：
        ```shell
        logdevice regionsize UUID [[no]sync] [block_on_error]
        ```
 - #log_args
    将指定的日志参数的个数。
 - logargs
    日志参数；参数个数由`#log-args`参数指定，有效的日志参数由`log_type`参数确定。
 - #devs
    镜像的副本数；为每个副本指定一个设备和一个偏移量。
 - device
    块设备，文件系统中的设备名称或`major:minor`格式。
 - offset
    块设备上的起始偏移量。

## snapshot and snapshot-origin target
### snapshot
```shell
start length snapshot origin COW-device P|N chunksize
```

### snapshot-origin
```shell
start length snapshot-origin origin
```

## error target
对映射扇区的任何I/O操作均会失败。
```shell
start length error
```

## zero target
等效于`/dev/zero`的块设备。对映射扇区的读操作返回零块；写入映射扇区的数据将被丢弃，但写入成功。
```shell
start length zero
```

## multipath target
支持多路径设备的映射。
```shell
start length multipath #features [feature1 ... featureN] #handlerargs [handlerarg1 ... handlerargN] #pathgroups pathgroup pathgroupargs1 ... pathgroupargsN
```
 - start
    虚拟设备中的起始块。
 - length
    该段的长度。
 - #features
    多路径`features`参数个数，后跟`features`参数。如果`#features`为0，则没有`features`参数，并且下一个参数是`#handlerargs`。目前，可以使用`multipath.conf`文件中的`no_path_retry`的`features`属性设置一项受支持的功能。这表明如果没有可用路径，则此多路径设备当前设置为排队I/O操作。
    在以下示例中，`multipath.conf`文件中的`no_path_retry`属性已设置为仅对I/O操作进行排队，直到在尝试使用路径达到设定次数后所有路径都被标记为失败。在这种情况下，映射如下所示，直到所有路径检查器都未通过指定数量的检查。
    ```shell
    0 71014400 multipath 1 queue_if_no_path 0 2 1 round-robin 0 2 1 66:128 \
    1000 65:64 1000 round-robin 0 2 1 8:0 1000 67:192 1000
    ```
    在所有路径检查器都未通过指定数量的检查后，映射将如下所示。
    ```shell
    0 71014400 multipath 0 0 2 1 round-robin 0 2 1 66:128 1000 65:64 1000 \
    round-robin 0 2 1 8:0 1000 67:192 1000
    ```
 - #handlerargs
    硬件处理程序参数的数量，后跟这些参数。硬件处理程序指定一个模块，该模块将用于在切换路径组或处理I/O错误时执行特定于硬件的操作。如果设置为0，则下一个参数是#pathgroups。
 - #pathgroups
    路径组的数量。路径组是多路径设备将在其上进行负载平衡的一组路径。每个路径组都有一组`pathgroupargs`参数。
 - pathgroup
    下一个要尝试的路径组。
 - pathgroupsargs
    每个路径组由以下参数组成：
    ```shell
    pathselector #selectorargs #paths #pathargs device1 ioreqs1 ... deviceN ioreqsN 
    ```
    + pathselector
        指定用于确定此路径组中的哪个路径用于下一次I/O操作的算法。
    + #selectorargs
        多路径映射中此参数之后的路径选择器参数的数量。目前，此参数的值始终为0。
    + #paths
        此路径组中的路径数。
    + #pathargs
        为此组中的每个路径指定的路径参数的数量。目前，这个数字始终为1，即ioreqs参数。
    + device
        路径的块设备号，`major:minor`格式。
    + ioreqs
        在切换到当前组中的下一个路径之前路由到该路径的I/O请求数。

## crypt target
对通过指定设备的数据进行加密。它使用内核Crypto API。
```shell
start length crypt cipher key IV-offset device offset
```
 - start
    虚拟设备中的起始块。
 - length
    该段的长度。
 - cipher
    由`cipher[-chainmode]-ivmode[:iv options]`组成。
    + cipher
        `/proc/crypto`中列出的可用密码（例如，aes）。
    + chainmode
        始终使用cbc。不要使用ebc；它不使用初始向量（IV）。
    + ivmode[:iv options]
        IV是用于改变加密的初始向量。IV模式是`plain`或`essiv:hash`。`-plain`的`ivmode`使用扇区号（加上IV偏移）作为IV。`-essiv`的`ivmode`是避免水印缺陷的增强。
 - key
    加密密钥，以十六进制形式提供。
 - IV-offset
    初始向量（IV）偏移。
 - device
    块设备，文件系统中的设备名称或`major:minor`格式。
 - offset
    块设备上的起始偏移量。



# 参考
 * [dmsetup(8) - Linux manual page](https://man7.org/linux/man-pages/man8/dmsetup.8.html)
 * [Appendix A. The Device Mapper Red Hat Enterprise Linux 7 | Red Hat Customer Portal](https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/7/html/logical_volume_manager_administration/device_mapper)
