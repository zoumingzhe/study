[bcache](https://bcache.evilpiepirate.org/)
===========================================
bcache（block cache）允许将SSD用作另一块设备（通常是机械盘盘或阵列）的读/写缓存（writeback）或读缓存（writethrough或writearound）。

# 警告
***操作前请备份重要数据。***
 - 磁盘格式在3.18中发生了变化，先前格式不向后兼容。注意：这只适用于使用bcache-dev分支编译的用户，内置到Linux内核的版本不受影响。

# 安装bcache
Bcache从3.10开始正式并入内核主线，通过`uname -r`命令查看内核版本号。
通过查看是否存在`/sys/fs/bcache`目录确定bcache是否存在于内核中。
通过查看是否存在`/lib/modules/<$version>/kernel/drivers/md/bcache`目录确定bcache是否以内核模块方式存在。

## 加载内核模块
如果bcache不存在于内核中，但是以内核模块方式存在，则可以加载内核模块。
`sudo modprobe bcache`

## 编译与安装内核
如果bcache不存在于内核中，也不以内核模块方式存在，则需要编译并安装内核。
### 下载内核
`wget "https://vault.centos.org/8.0.1905/BaseOS/Source/SPackages/kernel-4.18.0-80.7.1.el8_0.src.rpm"`
### 安装内核RPM包
```
mkdir -p ~/rpmbuild/{BUILD,BUILDROOT,RPMS,SOURCES,SPECS,SRPMS}
echo "%_topdir $(echo $HOME)/rpmbuild" > ~/.rpmmacros
# rpm2cpio kernel-4.18.0-80.7.1.el8_0.src.rpm | cpio -idmv 
rpm -ivh kernel-4.18.0-80.7.1.el8_0.src.rpm 2>&1 | grep -v exist
```
### 解压内核源码包
```
cd ~/rpmbuild/SOURCES/
xz -d linux-4.18.0-80.7.1.el8_0.tar.xz
tar xvf linux-4.18.0-80.7.1.el8_0.tar
cd linux-4.18.0-80.7.1.el8_0
```
### 修改内核配置
```
make mrproper
cp /boot/config-$(uname -r) ./.config
vim .config
```
### 编译内核
`make rpm -j 32`

## 安装bcache-tools
### Ubuntu
`sudo apt-get install bcache-tools`
### CentOS
#### 安装编译依赖
`yum -y install pkg-config libblkid-devel`
#### 获取源码
`git clone http://evilpiepirate.org/git/bcache-tools.git`
#### 检出版本
`git checkout v1.0.8`
#### 编译安装
`make && make install`

# bcache命令解析
## 擦除超块
`wipefs -a <dev>`

## 格式化硬盘分区
### 格式化数据盘
`make-bcache -B <dev>`
### 格式化缓存盘
`make-bcache -C <dev>`

## 注册与注销
### 设备注册
格式化完成后需要完成设备的注册，使内核获取设备。
`echo <dev> > /sys/fs/bcache/register`
也可以通过下面的方式实现设备自动注册：
`echo /dev/sd* > /sys/fs/bcache/register_quiet`
### 设备注销
`echo 1 > /sys/fs/bcache/<CSET-UUID>/unregister`

## 绑定与解绑
完成注册的backing设备需要在使用之前绑定到bcache，否则功能无法启用。
### 查看CSET-UUID
`bcache-super-show <dev>`
### 设备绑定
`echo <CSET-UUID> > /sys/block/bcache<N>/bcache/attach`
### 设备解绑
`echo <CSET-UUID> > /sys/block/bcache<N>/bcache/detach`

## 停用与启用
### 设备停用
`echo 1 > /sys/block/bcache<N>/bcache/stop`

## 查看信息
### 查看设备信息
`bcache-super-show -f <dev>`
### 查看state
`cat /sys/block/bcache<N>/bcache/state`
 - no cache：该backing设备没有attach任何caching设备
 - clean：一切正常，缓存是干净的
 - dirty：一切正常，已启用回写，缓存是脏的
 - inconsistent：遇到问题，后台设备与缓存设备不同步
### 查看缓存数据量
`cat /sys/block/bcache<N>/bcache/dirty_data`
### 查看writeback信息
`cat /sys/block/bcache0/bcache/writeback_`

# bcache缓存策略
bcache支持三种缓存策略：writeback、writethrough、writearoud，缓存策略可动态修改，默认使用writethrough。
 - writeback（写回）：数据先写入缓存盘，然后等待系统将数据回写入后端数据盘中。
 - writethrough（写通）：数据同时写入缓存盘和后端数据盘。
 - writearoud：数据直接写入后端数据盘。
## 查看缓存策略
`cat /sys/block/bcache<N>/bcache/cache_mode`
## 更改缓存策略
`echo <cache_mode> > /sys/block/bcache<N>/bcache/cache_mode`
## 写命中write-hit
对于writeback，先写入缓存盘，并使用dirty标志位记录缓存的修改。
对于writeback，先写入缓存盘，再写入数据盘。
对于writearoud，先使用dirty标志位记录缓存的修改，再写入数据盘。
## 写缺失write-miss
 - write allocate：将写入位置读入缓存盘，然后执行write-hit（写命中）操作。
 - no-write allocate：并不将写入位置读入缓存盘，而是直接将数据写入数据盘。
无论是writethrough还是writeback都可以使用写缺失的两种方式之一。只是通常writeback采用write allocate方式，而writethrough采用no-write allocate方式。因为在短时间内对同一缓存多次写入时，对于writeback使用write allocate可以提升性能，而对于writethrough则没有帮助。

# 参考
 * [Bcache - ArchWiki](https://wiki.archlinux.org/title/bcache)
 * [bcache kernel documentation](https://www.kernel.org/doc/Documentation/bcache.txt)
 * [bcache配置方法 - Linux内核之bcache 1.1](https://blog.csdn.net/liangchen0322/article/details/50322635)
 * [Linux下块设备缓存之Bcache使用(整理)](https://markrepo.github.io/maintenance/2018/09/10/bcache/)
 * [bcache的使用](https://www.cnblogs.com/sunhaohao/archive/2017/07/03/sunhaohao.html)
 * [bcache使用教程](https://ziyablog.com/266/bcache%E4%BD%BF%E7%94%A8%E6%95%99%E7%A8%8B/)
 * [bcache / 如何使用bcache构建LVM,软RAID / 如何优化bcache](https://developer.aliyun.com/article/60734)
 * [centos7 build centos8的v4.18 kernel 带bcache](https://zhuanlan.zhihu.com/p/90558003)
 * [智能预取](https://support.huaweicloud.com/ug-ip-kunpengaccel/ug-ip-kunpengaccel.pdf)
 * [鲲鹏 BoostKit 分布式存储使能套件](https://support.huaweicloud.com/usermanual-kunpengsdss/kunpengsdss-usermanual.pdf)
