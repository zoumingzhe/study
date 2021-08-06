tail
====
Usage: tail [OPTION]... [FILE]...
Print the last 10 lines of each FILE to standard output.
With more than one FILE, precede each with a header giving the file name.

**参数：**
 * -c, --bytes=[+]NUM       输出最后NUM字节；或者使用`-c +NUM`从每个文件的第NUM字节开始输出
 * -n, --lines=[+]NUM       输出最后NUM行，而不是最后10行；或者使用`-n +NUM`从每个文件的第NUM行开始输出
 * -f, --follow[={name|descriptor}] 随着文件的增长，输出附加的数据；缺失默认参数为“descriptor”
 * -s, --sleep-interval=N   使用-f时，在每次反复的间隔休眠N秒（默认值1.0）；对于inotify和`--pid=P`，至少每N秒检查一次进程P
 * -q, --quiet, --silent    从不输出给出文件名的标题
 * -v, --verbose            始终输出给出文件名的标题
 * -z, --zero-terminated    行分隔符是NUL，不是换行符
 * --max-unchanged-stats=N  使用`--follow=name`时，重新打开一个在N（默认值5）次迭代后大小没有改变的文件，查看它是否被取消链接或重命名（这是旋转日志文件的常见情况）；对于inotify，此选项很少有用
 * --pid=PID                使用-f时，表示在进程ID,PID死亡之后结束
 * --retry                  如果无法访问文件，继续尝试打开该文件

# 参考
 * [Linux tail 命令](https://www.runoob.com/linux/linux-comm-tail.html)
