taskset
=======
Usage: taskset [选项] [掩码 | cpu列表] [pid|命令 [参数...]]

taskset命令用于显示或更改某个进程的 CPU 亲和性。

Options:
 -a, --all-tasks         operate on all the tasks (threads) for a given pid
                         在给定 pid 的所有任务(线程)上操作
 -p, --pid               operate on existing given pid
                         在存在的给定 pid 上操作
 -c, --cpu-list          display and specify cpus in list format
                         以列表格式显示和指定 CPU
 -h, --help              display this help
                         显示此帮助
 -V, --version           display version
                         输出版本信息

默认行为是运行一条新命令：
    taskset 03 sshd -b 1024
您可以获取现有任务的掩码：
    taskset -p 700
或设置掩码：
    taskset -p 03 700
使用逗号分隔的列表格式而不是掩码：
    taskset -pc 0,3,7-11 700
列表格式中的范围可以带一个跨度参数：
    例如 0-31:2 与掩码 0x55555555 等效


# 参考
 * []()
