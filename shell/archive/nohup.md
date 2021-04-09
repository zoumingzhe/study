# nohup
nohup command [arg]… [&]
 - command：执行命令。
 - arg：命令参数。
 - &：后台执行命令，终端退出后命令不终止。

nohup（英文全称no hang up，不挂起）命令用于在系统后台不挂断地运行命令，退出终端不会影响程序的运行。

nohup命令在默认情况下（非重定向时）会输出一个nohup.out文件到当前目录下，如果当前目录的nohup.out文件不可写，输出重定向到$HOME/nohup.out文件中。

# 参考
1.[Linux nohup 命令](https://www.runoob.com/linux/linux-comm-nohup.html)
