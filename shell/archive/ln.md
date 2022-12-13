ln (link)
=========
Usage: ln [OPTION]... [-T] TARGET LINK_NAME
  or:  ln [OPTION]... TARGET
  or:  ln [OPTION]... TARGET... DIRECTORY
  or:  ln [OPTION]... -t DIRECTORY TARGET...
In the 1st form, create a link to TARGET with the name LINK_NAME.
In the 2nd form, create a link to TARGET in the current directory.
In the 3rd and 4th forms, create links to each TARGET in DIRECTORY.
Create hard links by default, symbolic links with --symbolic.
By default, each destination (name of new link) should not already exist.
When creating hard links, each TARGET must exist.  Symbolic links can hold arbitrary text; if later resolved, a relative link is interpreted in relation to its parent directory.

ln（link）命令为某一个文件在另外一个位置建立一个同步的链接。

# 硬链接（hard link）
通过索引节点来进行的链接。在Linux的文件系统中，保存在磁盘分区中的文件不管是什么类型都给它分配一个编号，称为索引节点号（Inode Index）。在Linux中，多个文件名指向同一索引节点是存在的。硬链接的作用是允许一个文件拥有多个有效路径名，这样用户就可以硬链接到重要文件，以防止“误删”。因为若目录的索引节点有一个以上的链接，只删除一个链接并不影响索引节点本身和其它的链接，只有当最后一个链接被删除，文件的数据块及目录的链接才会被释放。也就是说，此时文件才会被真正删除。
 - 硬链接以文件副本的形式存在。但不占用实际空间。
 - 硬链接只能链接文件，不允许链接目录。
 - 硬链接只有在同一个文件系统中才能创建。

# 软链接（symbolic link）
是一个特殊的文件，它的内容不是真正的数据，而是指向另一个文件（链接目标）的路径名。在符号链接中，文件实际上是一个文本文件，其中包含的有另一文件的位置信息。对符号链接的大部分操作（打开、读、写等），都被传递给其链接目标，操作真正作用在链接目标上，另外一些操作（如删除等）则作用在符号链接本身。
 - 软链接以路径的形式存在，类似于Windows操作系统中的快捷方式。
 - 软链接可以对一个不存在的文件名进行链接。
 - 软链接可以对目录进行链接。
 - 软链接可以跨文件系统。


# 参考
 * [Linux ln 命令](https://www.runoob.com/linux/linux-comm-ln.html)
 * [shell ln](https://www.cnblogs.com/chenhuan001/p/6306735.html)
