[bcache](https://bcache.evilpiepirate.org/)
===========================================
bcache（block cache）允许将SSD用作另一块设备（通常是机械盘盘或阵列）的读/写缓存（writeback）或读缓存（writethrough或writearound）。

# 警告
***操作前请备份重要数据。***
 - 磁盘格式在3.18中发生了变化，先前格式不向后兼容。注意：这只适用于使用bcache-dev分支编译的用户，内置到Linux内核的版本不受影响。

# bcache安装
Bcache从3.10开始正式并入内核主线，通过`uname -r`命令查看内核版本号。
## bcache-tools安装


# bcache命令解析
## 创建后端盘
`make-bcache -B <dev>`
## 创建缓存盘
`make-bcache -C <dev>`
## 添加缓存盘
`echo <CSET-UUID> >/sys/block/bcache<N>/bcache/attach`
## 删除缓存盘
`echo <CSET-UUID> >/sys/block/bcache<N>/bcache/detach`

# 参考
 * [Bcache - ArchWiki](https://wiki.archlinux.org/title/bcache)
 * [bcache kernel documentation](https://www.kernel.org/doc/Documentation/bcache.txt)
 * [Linux下块设备缓存之Bcache使用(整理)](https://markrepo.github.io/maintenance/2018/09/10/bcache/)
