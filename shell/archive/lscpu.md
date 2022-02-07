lscpu
=====
Usage:
 lscpu [options]

Display information about the CPU architecture.

lscpu命令用于显示 CPU 架构信息。

Options:
 -a, --all               同时打印在线和离线 CPU (-e 选项默认值)
 -b, --online            只打印在线 CPU (-p 选项默认值)
 -c, --offline           只打印离线 CPU
 -J, --json              use JSON for default or extended format
 -e, --extended[=<列表>] 打印扩展的可读格式
 -p, --parse[=<列表>]    打印可解析格式
 -s, --sysroot <目录>    以指定目录作为系统根目录
 -x, --hex               打印十六进制掩码而非 CPU 列表
 -y, --physical          print physical instead of logical IDs
                         打印物理 ID 而非逻辑 ID

 -h, --help     显示此帮助并退出
 -V, --version  输出版本信息并退出

可用的列：
           CPU  逻辑 CPU 数量
          CORE  逻辑核心数量
        SOCKET  逻辑(CPU)座数量
          NODE  逻辑 NUMA 节点数量
          BOOK  逻辑 book 数
        DRAWER  logical drawer number
                逻辑抽屉号
         CACHE  shows how caches are shared between CPUs
                显示 CPU 间是如何共享缓存的
  POLARIZATION  CPU dispatching mode on virtual hardware
                虚拟硬件上的 CPU 调度模式
       ADDRESS  physical address of a CPU
                CPU 的物理地址
    CONFIGURED  shows if the hypervisor has allocated the CPU
                显示超级监督(hypervisor)是否分配了 CPU
        ONLINE  shows if Linux currently makes use of the CPU
                显示 Linux 当前是否在使用该 CPU
        MAXMHZ  shows the maximum MHz of the CPU
                显示 CPU 的最大 MHz
        MINMHZ  shows the minimum MHz of the CPU
                显示 CPU 的最小 MHz


# 参考
 * []()
