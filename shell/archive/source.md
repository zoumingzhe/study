source
======

source（点）命令用于在当前shell中读取并执行filename文件中的命令。

`source filename [arg]`

 - `$PATH`变量中的条目被用于寻找包含`FILENAME`文件的目录。
 - 如果提供了任何的`ARGUMENTS`参数，则它们将成为`FILENAME`文件执行时的位置参数。

*注：该命令通常用命令“.”来替代。
```shell
source filename
. filename #（中间有空格）
```

source filename 与 sh filename 及 ./filename 执行脚本的区别：
 - sh filename 重新建立一个子shell，在子shell中执行脚本里面的语句，该子shell继承父shell的环境变量，但子shell新建的、改变的变量不会被带回父shell。
 - ./filename 是因为当前目录没有在PATH中，所有”.”是用来表示当前目录的。当shell脚本具有可执行权限时，用 sh filename 与 ./filename 执行脚本是没有区别的。
 - source filename 只是简单地读取脚本里面的语句依次在当前shell里面执行，没有建立新的子shell，脚本里面所有新建、改变的变量都会保存在当前shell里面。

# 参考
 * [Linux下source命令详解](https://blog.csdn.net/violet_echo_0908/article/details/52056071)
 * [source命令](https://www.jianshu.com/p/63ded646d4cd)
