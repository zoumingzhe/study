git tag
=======

Git可以给仓库历史中的某一个提交打上标签，以示重要。

此功能通常用于来标记发布点（v1.0、v2.0等）。

标签类型
--------

Git支持两种标签：轻量标签（lightweight）与附注标签（annotated）。

轻量标签很像一个不会改变的分支——它只是某个特定提交的引用。

而附注标签是存储在Git数据库中的一个完整对象，它们是可以被校验的，其中包含打标签者的名字、电子邮件地址、日期时间，此外还有一个标签信息，并且可以使用GNU Privacy Guard（GPG）签名并验证。通常会建议创建附注标签，这样你可以拥有以上所有信息。但是如果你只是想用一个临时的标签，或者因为某些原因不想要保存这些信息，那么也可以用轻量标签。

tag相关命令
----------

### 列出标签

`git tag`

#### list选项（-l）

`git tag -l <tagname>`

### 创建标签

#### 轻量标签

`git tag <tagname>`

#### 附注标签

`git tag -a <tagname>`

##### message选项（-m）

`git tag -a <tagname> -m <message>`

#### 后期标签

如果忘记打标签，也可以使用`git tag <tagname> <checksum>`命令，对过去的提交打标签。

### 共享标签

默认情况下，`git push`命令并不会传送标签到远程仓库服务器上。

在创建完标签后必须执行`git push origin <tagname>`命令，显式地推送标签到远程仓库服务器上。

如果想要一次性推送很多标签，也可以使用`git push origin --tags`命令，这将会把所有不在远程仓库服务器上的标签全部传送到那里。

### 删除标签

要删除掉你本地仓库上的标签，可以使用命令`git tag -d <tagname>`。

### 检出标签

如果你想查看某个标签所指向的文件版本，可以使用`git checkout <tagname>`命令，或者使用`git checkout -b <branchname> <tagname>`检出至新的分支。

参考
----

- [2.6 Git Basics - Tagging](https://git-scm.com/book/en/v2/Git-Basics-Tagging)
- [2.6 Git 基础 - 打标签](https://git-scm.com/book/zh/v2/Git-%E5%9F%BA%E7%A1%80-%E6%89%93%E6%A0%87%E7%AD%BE)
