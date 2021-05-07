# udev
udev 是 Linux 内核的设备管理器。总的来说，它取代了devfs和hotplug，负责管理/dev中的设备节点。同时，udev也处理所有用户空间发生的硬件添加、删除事件，以及某些特定设备所需的固件加载。

与传统的顺序加载相比，udev通过并行加载内核模块提供了潜在的性能优势。异步加载模块的方式也有一个天生的缺点：无法保证每次加载模块的顺序，如果机器具有多个块设备，那么它们的设备节点可能随机变化。例如如果有两个硬盘，/dev/sda 可能会随机变成/dev/sdb。

# udev规则
规则文件是udev里最重要的部分，默认保存在/etc/udev/rules.d/目录下，其文件名必须以.rules结尾。各种软件包提供的规则文件位于/lib/udev/rules.d/目录中，如果/usr/lib和/etc目录中有同名.rules文件，则以/etc中的.rules文件优先。

默认规则文件不仅为设备产生内核设备名称，还会产生标识性强的符号链接。

## udev规则的操作符
 - “==”：比较键、值，若等于，则该条件满足；
 - “!=”：比较键、值，若不等于，则该条件满足；
 - “=”：对一个键赋值；
 - “+=”：为一个表示多个条目的键赋值。
 - “:=”：对一个键赋值，并拒绝之后所有对该键的改动。目的是防止后面的规则文件对该键赋值。

## udev规则的匹配键
 - ACTION：事件（uevent）的行为，例如：add（添加设备）、remove（删除设备）。
 - KERNEL：内核设备名称。例如：sda、cdrom。
 - DEVPATH：设备的devpath路径。
 - SUBSYSTEM：设备的子系统名称。例如：sda的子系统为block。
 - BUS：设备在devpath里的总线名称。例如：usb。
 - DRIVER：设备在devpath里的设备驱动名称。例如：ide-cdrom。
 - ID：设备在devpath里的识别号。
 - SYSFS{filename}：设备的devpath路径下，设备的属性文件“filename”里的内容。
 例如：SYSFS{model}==“ST936701SS”表示：如果设备的型号为ST936701SS，则该设备匹配该匹配键。
 在一条规则中，可以设定最多五条SYSFS的匹配键。
 - ENV{key}：环境变量。在一条规则中，可以设定最多五条环境变量的匹配键。
 - PROGRAM：调用外部命令。
 - RESULT：外部命令PROGRAM 的返回结果。
 例如：PROGRAM=="/lib/udev/scsi_id-g -s $devpath", RESULT=="35000c50000a7ef67"
 调用外部命令/lib/udev/scsi_id查询设备的SCSIID，如果返回结果为35000c50000a7ef67，则该设备匹配该匹配键。

## udev规则的赋值键
 - NAME：在/dev下产生的设备文件名。只有第一次对某个设备的NAME的赋值行为生效，之后匹配的规则再对该设备的NAME赋值行为将被忽略。如果没有任何规则对设备的NAME赋值，udev将使用内核设备名称来产生设备文件。
 - SYMLINK：为/dev/下的设备文件产生符号链接。由于udev只能为某个设备产生一个设备文件，所以为了不覆盖系统默认的udev规则所产生的文件，推荐使用符号链接。
 - OWNER、GROUP、MODE：为设备设定权限。
 - ENV{key}：导入一个环境变量。

## udev规则的值和替换操作符
 - $kernel或%k：设备的内核设备名称。例如：sda、cdrom。
 - $number或%n：设备的内核号码。例如：sda3的内核号码是3。
 - $devpath或%p：设备的devpath路径。
 - $id或%b：设备在devpath里的ID号。
 - $sysfs{file}或%s{file}：设备的sysfs里file的内容（即设备的属性值）。例如：$sysfs{size}表示该（磁盘）设备的大小。
 - $env{key}或%E{key}：环境变量key的值。
 - $major或%M：设备的major号。
 - $minor或%m：设备的minor号。
 - $result或%c：PROGRAM返回的结果。
 - $parent或%P：父设备的设备文件名。
 - $root或%r：udev_root的值，默认是/dev/。
 - $tempnode或%N：临时设备名。
 - %%：符号%本身。
 - $$：符号$本身。

# 参考
 * [udev (简体中文)](https://wiki.archlinux.org/title/Udev_(%E7%AE%80%E4%BD%93%E4%B8%AD%E6%96%87))
 * [Writing udev rules](http://www.reactivated.net/writing_udev_rules.html)
 * [Linux下udev详细介绍](https://blog.51cto.com/seiang/1950594)
