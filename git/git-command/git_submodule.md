git submodule
=============

检查、更新和管理子模块。

为项目添加一个新的子模块
----------------------

通过在`git submodule add`命令添加新的子模块（一个已存在的`git`仓库）：

```shell
git submodule add <repo>
```

克隆一个含有子模块的项目
----------------------

当克隆一个含有子模块的项目（`git clone <repo>`），默认会包含该子模块目录，但其中还没有任何文件。

你必须运行两个命令：

- `git submodule init` 初始化本地配置文件；
- `git submodule update` 从该项目中抓取所有数据并检出父项目中列出的合适的提交。

更简单的方式，在`git clone`命令中增加`--recurse-submodules`选项，它就会自动初始化并更新仓库中的每一个子模块，包括可能存在的嵌套子模块。

```shell
git clone --recurse-submodules <repo>
```

参考
----

- [Git - git-submodule Documentation](https://git-scm.com/docs/git-submodule)
- [Git - 子模块](https://git-scm.com/book/zh/v2/Git-%E5%B7%A5%E5%85%B7-%E5%AD%90%E6%A8%A1%E5%9D%97)
