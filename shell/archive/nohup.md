nohup
=====
Usage: nohup COMMAND [ARG]...
  or:  nohup OPTION
Run COMMAND, ignoring hangup signals.
 - COMMAND：执行命令。
 - ARG：命令参数。

nohup（no hang up，不挂起）命令用于在系统后台不挂断地运行命令，退出终端不会影响程序的运行。

nohup命令在默认情况下（非重定向时）会输出一个nohup.out文件到当前目录下，如果当前目录的nohup.out文件不可写，输出重定向到$HOME/nohup.out文件中。

# 参考
 * [Linux nohup 命令](https://www.runoob.com/linux/linux-comm-nohup.html)
