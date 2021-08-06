linux dev
=========

# major and minor
每个设备节点的类型（block或character）和数字作为内核的标识符。

在Linux上，设备的规范列表以及对其功能的简要说明都保存在内核中。

查看配置到当前运行内核中的（block或character）设备驱动程序列表：
`cat /proc/devices`

## major number
识别与设备关联的驱动程序。例如`/dev/null`和`/dev/zero`都由驱动程序1管理，而虚拟控制台和串行终端则由驱动程序4管理。

## minor number
引用一个实例，由驱动程序本身使用，由`major number`指定。`minor number`用于驱动程序识别设备之间的差异。

# 参考
 * [《Linux Device Drivers, Second Edition by Jonathan Corbet, Alessandro Rubini》]()
 * [Linux 设备中的 major 和 minor 数字](http://einverne.github.io/post/2020/06/linux-device-major-and-minor-number.html)
 * [Linux的主设备号和次设备号](http://blog.chinaunix.net/uid-24460251-id-2606762.html)
 * [linux设备号详解](https://blog.csdn.net/zjjyliuweijie/article/details/7001383)
