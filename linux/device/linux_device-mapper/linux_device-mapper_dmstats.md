dmstats — device-mapper statistics management
=============================================

`dmstats`管理`device-mapper`设备的IO统计区域。

使用该工具可以创建、删除、列出和报告统计区域。

`dmstats`的第一个参数是一个命令；第二个参数是设备名称、`uuid`或`major`和`minor`。

更多选项允许选择区域、输出格式控制和报告行为。

当没有给定设备参数时，默认情况下`dmstats`将在所有存在的`device-mapper`设备上运行。以这种方式使用时，创建和删除命令需要使用`--alldevices`选项。

# 参考
 * [dmstats(8) - Linux manual page](https://man7.org/linux/man-pages/man8/dmstats.8.html)
