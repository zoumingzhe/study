git config
==========

获取和设置仓库或全局的选项。

配置文件位置
----------

- local，仓库配置文件，`.git/config`
- global，全局配置文件，`~/.gitconfig`
- system，系统配置文件，`${prefix}/etc/gitconfig`

core
----

### core.filemode

指定是否检查文件权限。
当配置文件中未指定 core.filemode 时，默认值为 true。
使用 Git for Windows 或 Eclipse 创建存储库时，可能需要将此变量设置为 false。

### core.symlinks

指定是否处理符号连接。
默认为 true，如果为 false，符号链接将作为包含链接文本的小纯文件检出。

### core.ignorecase

指定是否忽略大小写，以使 Git 在不区分大小写的文件系统（如 APFS、HFS+、FAT、NTFS 等）上更好地工作。
当配置文件中未指定 core.ignorecase 时，默认为 false。

sendemail
---------

通常，可在全局配置（`~/.gitconfig`）中增加如下配置段：

```text
[sendemail]
        smtpEncryption = tls
        smtpServer = smtp.gmail.com
        smtpServerPort = 587
        smtpUser = username@gmail.cn
        smtpPass = password
```

关于smtp的更多配置信息（如域名、服务端口等），可参考邮件服务提供商的帮助文档。

参考
----

- [Git - git-config Documentation](https://git-scm.com/docs/git-config)
