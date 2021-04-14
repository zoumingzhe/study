# chmod
Usage: chmod [OPTION]... MODE[,MODE]... FILE...
  or:  chmod [OPTION]... OCTAL-MODE FILE...
  or:  chmod [OPTION]... --reference=RFILE FILE...
Change the mode of each FILE to MODE.

chmod（change mode）命令用于修改用户对文件的权限。

 - Linux/Unix的文件调用权限分为三级：文件所有者（Owner）、用户组（Group）、其它用户（Other Users）。
 - 只有文件所有者和超级用户可以修改文件或目录的权限。


|  #  | 权限          | rwx | 二进制 |
| :-: | :------------ | :-: | :---: |
|  7  | 读 + 写 + 执行 | rwx |  111  |
|  6  | 读 + 写       | rw- |  110  |
|  5  | 读 + 执行      | r-x |  101  |
|  4  | 读            | r-- |  100  |
|  3  | 写 + 执行      | -wx |  011  |
|  2  | 写            | -w- |  010  |
|  1  | 执行           | --x |  001  |
|  0  | 无            | --- |  000  |

# 参考
1.[Linux chmod 命令](https://www.runoob.com/linux/linux-comm-chmod.html)
