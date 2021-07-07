[bcache](https://bcache.evilpiepirate.org/)
===========================================
bcache（block cache）允许将SSD用作另一块设备（通常是机械盘盘或阵列）的读/写缓存（writeback）或读缓存（writethrough或writearound）。

# 警告
***操作前请备份重要数据。***
 - 磁盘格式在3.18中发生了变化，先前格式不向后兼容。注意：这只适用于使用bcache-dev分支编译的用户，内置到Linux内核的版本不受影响。

# bcache安装
Bcache从3.10开始正式并入内核主线，通过`uname -r`命令查看内核版本号。
## 安装编译依赖
`yum -y install pkg-config libblkid-devel`
## 编译安装bcache-tools
### 获取源码
`git clone http://evilpiepirate.org/git/bcache-tools.git`
### 编译安装
`make && make install`

# bcache命令解析
## 擦除超块
`wipefs -a <dev>`
## 创建数据盘
`make-bcache -B <dev>`
## 创建缓存盘
`make-bcache -C <dev>`
## 添加缓存盘
`echo <CSET-UUID> > /sys/block/bcache<N>/bcache/attach`
## 删除缓存盘
`echo <CSET-UUID> > /sys/block/bcache<N>/bcache/detach`
## 查看CSET-UUID
`bcache-super-show <dev>`

# bcache缓存策略
bcache支持三种缓存策略：writeback、writethrough、writearoud，缓存策略可动态修改，默认使用writethrough。
 - writeback（写回）：数据先写入缓存盘，然后等待系统将数据回写入后端数据盘中。
 - writethrough（写通）：数据同时写入缓存盘和后端数据盘。
 - writearoud：数据直接写入后端数据盘。
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
 * [Linux下块设备缓存之Bcache使用(整理)](https://markrepo.github.io/maintenance/2018/09/10/bcache/)
