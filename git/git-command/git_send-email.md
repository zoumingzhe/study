git send-email
==============

以电子邮件的形式发送补丁集。

接受命令行上给出的补丁并通过电子邮件发送出去。补丁可以指定为文件、目录（将发送目录中的所有文件），或直接指定为修订列表。在最后一种情况下，任何`git format-patch`接受的格式和选项都可以传递给`git send-email`。

电子邮件的标题可通过命令行选项进行配置。如果未在命令行中指定，将通过ReadLine界面提示用户提供必要的信息。

补丁文件接受两种格式：

 1. mbox 格式文件
这是`git format-patch`生成的。大多数标题和 MIME 格式都会被忽略。
 2. Greg Kroah-Hartman 的send_lots_of_email.pl 脚本使用的原始格式
这种格式要求文件的第一行包含“Cc:”，并且消息的“Subject:”作为第二行。

## 选项

- --in-reply-to \<identifier>
使第一封邮件（或所有带有`--no-thread`的邮件）显示为对给定Message-Id的回复，从而避免中断线程以提供新的补丁集。第二封和随后的电子邮件将根据`--[no-]chain-reply-to`设置作为回复发送。

## 参考

- [Git - git-send-email Documentation](https://git-scm.com/docs/git-send-email)
- [git send-email](https://cloud.tencent.com/developer/section/1138666)
