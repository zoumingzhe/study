# shell
Shell俗称壳（用来区别于核），是指“为使用者提供操作界面”的软件（命令解析器）。
它既是一种命令语言，又是一种程序设计语言。作为命令语言，它交互式解释和执行用户输入的命令或者自动地解释和执行预先设定好的一连串的命令；作为程序设计语言，它定义了各种变量和参数，并提供了许多在高级语言中才具有的控制结构，包括循环和分支。

# shell脚本
Shell脚本（shell script）以“.sh”作为后缀名，是一种为shell编写的脚本程序。

# shell解释器
Unix/Linux的Shell种类众多，常见的有：
 - Bourne Shell（/usr/bin/sh或/bin/sh）
 - [Bourne Again Shell（/bin/bash）](./bash.md)
 - C Shell（/usr/bin/csh）
 - K Shell（/usr/bin/ksh）
 - Shell for Root（/sbin/sh）

## Shebang(#!)
Shebang（也称为Hashbang）是一个由井号和叹号构成的字符序列#!，其出现在文本文件的第一行的前两个字符。

在文件中存在Shebang的情况下，类Unix操作系统的程序载入器会分析Shebang后的内容，将这些内容作为解释器指令，并调用该指令，并将载有Shebang的文件路径作为该解释器的参数。

例如，以指令#!/bin/sh开头的文件在执行时会实际调用/bin/sh程序（通常是Bourne shell或兼容的shell，例如bash、dash等）来执行。这行内容也是shell脚本的标准起始行。

# 目录
 - [awk](./archive/awk.md)
 - [cat](./archive/cat.md)
 - [cd](./archive/cd.md)
 - [chmod](./archive/chmod.md)
 - [cp](./archive/cp.md)
 - [grep](./archive/grep.md)
 - [kill](./archive/kill.md)
 - [ls](./archive/ls.md)
 - [mkdir](./archive/mkdir.md)
 - [more](./archive/more.md)
 - [mv](./archive/mv.md)
 - [nohup](./archive/nohup.md)
 - [ps](./archive/ps.md)
 - [pwd](./archive/pwd.md)
 - [rm](./archive/rm.md)
 - [rmdir](./archive/rmdir.md)
 - [sed](./archive/sed.md)
 - [source](./archive/source.md)
 - [su](./archive/su.md)

# 参考
1.[shell](https://baike.baidu.com/item/shell)

2.[Shell脚本](https://baike.baidu.com/item/Shell%E8%84%9A%E6%9C%AC)

3.[unix shell](https://baike.baidu.com/item/Unix%20shell)

4.[Shell 教程](https://www.runoob.com/linux/linux-shell.html)

5.[Shebang](https://zh.wikipedia.org/zh-hans/Shebang)
