Git for Windows
===============

Windows 下文件名大小写问题
------------------------

Linux下文件名是区分大小写的，而Windows下文件名不区分大小写。
从 Windows 10 （1803版本）开始，可以在文件夹级别启用大小写敏感属性。

### Windows 下 clone Linux 代码

#### 解决方案1：使用git命令重命名，而非Windows文件系统重命名

```shell
git mv <OldFileName> <newfilename>
```

#### 解决方案2：配置git对文件名敏感

为文件夹启用大小写敏感属性，请在终端中执行以下命令：

```PowerShell
fsutil.exe file SetCaseSensitiveInfo <path> enable
```

***需要注意的是，该配置只对目录下的新创建文件夹有效。***

执行以上命令的环境要求（缺一不可）：

* 所在分区为 NTFS 格式
* Windows 10 （1803版本）及以后版本
* 安装有 Linux 子系统，即 Windows Subsystem for Linux（WSL）
* 以管理员权限运行 PowerShell

Windows 下的相关配置
--------------------

* 文件权限：[core.filemode](../git-command/git_config.md#corefilemode)
* 符号连接：[core.symlinks](../git-command/git_config.md#coresymlinks)
* 大小写：[core.ignorecase](../git-command/git_config.md#coreignorecase)

参考
----

* [Git - git-config Documentation](https://git-scm.com/docs/git-config)
