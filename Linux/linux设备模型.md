# Linux设备模型
Linux为了降低设备多样性带来的驱动开发复杂度，以及设备热拔插处理、电源管理等，Linux内核提出了设备模型（Driver Model）的概念。

设备模型将硬件设备归纳、分类，然后抽象出一套标准的数据结构和接口。驱动的开发，就简化为对内核所规定的数据结构的填充和实现。

# Linux设备模型的基本概念
Bus（总线）：Linux认为（可以参考include/linux/device.h中struct bus_type的注释），<u>总线是CPU和一个或多个设备之间信息交互的通道</u>。而为了方便设备模型的抽象，所有的设备都应连接到总线上（无论是CPU内部总线、虚拟总线，还是“platform Bus”）。

Class（分类）：在Linux设备模型中，Class的概念非常类似面向对象程序设计中的Class（类），它主要是集合具有相似功能或属性的设备，这样就可以抽象出一套可以在多个设备之间共用的数据结构和接口函数。因而从属于相同Class的设备的驱动程序，就不再需要重复定义这些公共资源，直接从Class中继承即可。

Device（设备）：抽象系统中所有的硬件设备，描述它的名字、属性、从属的Bus、从属的Class等信息。

Device Driver（驱动）：Linux设备模型用Driver抽象硬件设备的驱动程序，它包含设备初始化、电源管理相关的接口实现。而Linux内核中的驱动开发，基本都围绕该抽象进行（实现所规定的接口函数）。

---
注：Platform Bus是Linux设备驱动模型为了保持设备驱动的统一性而虚拟出来的总线。在计算机中有这样一类设备，它们通过各自的设备控制器，直接和CPU连接，CPU可以通过常规的寻址操作访问它们（或者说访问它们的控制器）。这种连接方式，并不属于传统意义上的总线连接。但设备模型应该具备普适性，因此Linux就虚构了一条Platform Bus，供这些设备挂靠。

# kobject

# uevent
uevent是kobject的一部分，用于在kobject状态发生改变时，例如增加、移除等，通知用户空间程序。用户空间程序收到这样的事件后，会做相应的处理。

该机制通常是用来支持热拔插设备的，例如U盘插入后，USB相关的驱动软件会动态创建用于表示该U盘的device结构（相应的也包括其中的kobject），并告知用户空间程序，为该U盘动态的创建/dev/目录下的设备节点，更进一步，可以通知其它的应用程序，将该U盘设备mount到系统中，从而动态的支持该设备。

# sysfs
sysfs是一个基于RAM的文件系统，具备文件系统的所有属性。

sysfs和kobject一起，可以将kernel的数据结构以文件目录结构的形式，在用户空间提供对这些数据结构（以及数据结构的属性）的访问支持。

## sysfs和kobject的关系
sysfs中的目录描述了kobject，每一个kobject都会对应sysfs中的一个目录。

因此，将kobject添加到kernel时，create_dir接口会调用sysfs文件系统的创建目录接口，创建和kobject对应的目录。

# 参考
 * [统一设备模型 - 蜗窝科技](http://www.wowotech.net/sort/device_model)
 * [Linux设备模型(1)_基本概念](http://www.wowotech.net/device_model/13.html)
 * [Linux设备模型(2)_Kobject](http://www.wowotech.net/device_model/kobject.html)
 * [Linux设备模型(3)_Uevent](http://www.wowotech.net/linux_kenrel/uevent.html)
 * [Linux设备模型(4)_sysfs](http://www.wowotech.net/linux_kenrel/dm_sysfs.html)
 * [Linux设备模型(5)_device和device driver](http://www.wowotech.net/linux_kenrel/device_and_driver.html)
 * [Linux设备模型(6)_Bus](http://www.wowotech.net/linux_kenrel/bus.html)
 * [Linux设备模型(7)_Class](http://www.wowotech.net/linux_kenrel/class.html)
 * [Linux设备模型(8)_platform设备](http://www.wowotech.net/linux_kenrel/platform_device.html)
 * [Linux设备模型(9)_device resource management](http://www.wowotech.net/linux_kenrel/device_resource_management.html)
