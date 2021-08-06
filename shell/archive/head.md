head
====
Usage: head [OPTION]... [FILE]...
Print the first 10 lines of each FILE to standard output.
With more than one FILE, precede each with a header giving the file name.

**参数：**
 * -c, --bytes=[-]NUM       输出前NUM字节；或者使用`-c -NUM`输出至每个文件的最后NUM字节结束
 * -n, --lines=[-]NUM       输出前NUM行，而不是前10行；或者使用`-n -NUM`输出至每个文件的最后NUM行结束
 * -q, --quiet, --silent    从不输出给出文件名的标题
 * -v, --verbose            始终输出给出文件名的标题
 * -z, --zero-terminated    行分隔符是NUL，不是换行符

# 参考
 * [Linux head 命令](https://www.runoob.com/linux/linux-comm-head.html)
