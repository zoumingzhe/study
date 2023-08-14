linux crash
===========

trigger kernel crash
--------------------

```shell
echo c > /proc/sysrq-trigger
```

bt 查看堆栈
------------

显示帧中包含的所有堆栈数据，此选项可用于确定传递给每个函数的参数：

```shell
bt -f [pid | task]
```

或者，使用`-F`显示符号：

```shell
bt -F [pid | task]
```

查看所有堆栈：

```shell
foreach bt
```

sym 查看符号
------------

查看指定模块的符号：

```shell
sym -m <module>
```

list 打印链表
------------

若`start`为链表头，则使用`-H`选项可以忽略打印链表头：

```shell
list -H start
```

参考
----

* [crash-utility.github.io](https://crash-utility.github.io)
* [upstream git repository](https://github.com/crash-utility/crash.git)
* [调试工具（二）：crash（未完结） - DumpStack](http://www.dumpstack.cn/index.php/2022/02/23/395.html)
