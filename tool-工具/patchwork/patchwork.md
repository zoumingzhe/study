[patchwork](https://github.com/getpatchwork)
============================================

Patchwork 是一个基于 Web 的补丁跟踪系统，旨在促进对开源项目的贡献管理。

已发送到邮件列表的补丁会被系统“捕获”，并显示在网页上。任何引用补丁的评论也将附加到补丁页面。

项目的维护者可以浏览补丁列表，用特定的状态标记每个补丁，例如`Accepted`（已接受）、`Rejected`（已拒绝）或`Under Review`（正在审查中）。旧补丁可以发送到存档或删除。

[pwclient](https://github.com/getpatchwork/pwclient)
----------------------------------------------------

### 安装pwclient

安装`pwclient`及其依赖项的最简单方法是使用pip。请运行：

```shell
python3 -m pip install pwclient
```

### 配置pwclient

使用`pwclient`需要修改配置文件`~/.pwclientrc`。若连接`Linux Kernel`的`patchwork`，请修改配置：

```text
[options]
default = linux

[linux]
url = https://patchwork.kernel.org/xmlrpc/
```

### 下载patch

首先，根据邮件的标题，使用`pwclient search`查询补丁`patch id`。

```shell
pwclient search [-s STATE] [-a {yes,no}] [-p PROJECT] [-w WHO] [-d WHO] [-n MAX#] [-N MAX#] [-m MESSAGEID] [-f FORMAT] [STR]
```

然后，根据`patch id`，使用`pwclient get`下载补丁的patch文件。

```shell
pwclient get [-p PROJECT] PATCH_ID [PATCH_ID ...]
```

参考
----

* [patchwork](http://jk.ozlabs.org/projects/patchwork/)
* [如何Upstream kernel和U-boot - Rockchip Wiki](http://rockchip.wikidot.com/upstream-kernel-u-boot)
* [(3条消息) 如何从社区邮件列表获取完整的补丁（pwclient）_xiaofeng_yan的博客-CSDN博客](https://blog.csdn.net/xiaofeng_yan/article/details/109343716)
