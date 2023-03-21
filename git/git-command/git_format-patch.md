git format-patch
================

为电子邮件提交准备补丁集。

在每次提交的一个“message”中准备每个非合并提交及其“patch”，格式类似于UNIX邮箱。此命令的输出便于电子邮件提交或与`git am`一起使用。

命令生成的“message”由三部分组成：

- 以`From <commit>`开头的简短元数据标头，带有固定的`Mon Sep 17 00:00:00 2001`日期戳，以帮助“file”之类的程序识别文件是此命令的输出，字段记录作者身份、作者日期和更改标题（取自提交日志消息的第一段）。
- 提交日志消息的第二段和后续段落。
- “patch”，即提交与其父级之间的“diff -p --stat”输出（参见`git-diff`）。

选项
----

- -v, --reroll-count \<n>
将该补丁集标记为第几次迭代。输出文件名前面有“v\<n>”，主题前缀（默认情况下为“PATCH”，但可通过--subject prefix选项配置）后面有“v\<n>”。

参考
----

- [Git - git-format-patch Documentation](https://git-scm.com/docs/git-format-patch)
- [git format-patch](https://cloud.tencent.com/developer/section/1138664)
