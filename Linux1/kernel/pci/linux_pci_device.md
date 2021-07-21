linux pci device
================

# PCI设备寻址
<domain>:<bus>:<slot>.<func>
<vendor>:<device>:<class>

PCI设备寻址的三个元素：
 - 总线编号（bus，8bit）：每个系统最多256条总线
 - 插槽编号（slot，5bit）：每条总线最多32个设备
 - 功能编号（func，3bit）：每个设备最多8个功能

# 相关命令
## 显示pci设备
`lspci`

# 参考
 * [浅谈Linux PCI设备驱动（一）](https://blog.csdn.net/linuxdrivers/article/details/5849698)
