linux pci device
================

# PCI设备寻址
<domain>:<bus>:<slot>.<func>
<vendor>:<device>:<class>

PCI设备寻址的三个元素：
 - 总线编号（bus，8bit）：每个系统最多256条总线
 - 插槽编号（slot，5bit）：每条总线最多32个设备
 - 功能编号（func，3bit）：每个设备最多8个功能

# Linux PCI 总线扫描
在系统启动时，所有的PCI桥设备的配置寄存器的总线编号初始化为0，操作系统需要从根总线开始逐条为PCI总线分配编号。
## Linux PCI 总线编号
由主桥引出的PCI总线编号总是为0。

在实际中，存在以下两种情况：
 1. 在操作系统启动时，BIOS配置了一部分，并写到PCI桥设备的配置寄存器，操作系统只需要在BIOS基础上配置剩余部分。
 2. 在系统运行过程中，热插拔PCI桥设备或PCI设备，系统中核心的数据结构已经建立，没必要推倒重来。
为此，Linux操作系统执行两次PCI总线扫描：第一次扫描已分配总线编号的PCI桥，确定总线编号的已分配范围；第二次扫描未分配总线编号的PCI桥，并在第一次扫描的总线编号基础上为PCI桥分配总线编号。

## Linux PCI 总线扫描流程
 * [pci_scan_v3.10-rc7](pci_scan_v3.10-rc7.md)
 * [pci_scan_v4.18-rc9](pci_scan_v4.18-rc9.md)

# 相关命令
## 显示PCI设备
`lspci`

# 参考
 * [存储技术原理分析——基于Linux2.6内核源代码]()
 * [浅谈Linux PCI设备驱动（一）](https://blog.csdn.net/linuxdrivers/article/details/5849698)
