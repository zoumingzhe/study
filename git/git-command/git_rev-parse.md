git rev-parse
=============

挑选和转换参数。

选项
----

### --verify

确认只提供了一个参数，并且可以将其转换为可用于访问对象数据库的原始20字节SHA-1。如果是，则将其发送到标准输出；否则，报错。

请注意，如果验证来自不受信任来源的名称，可以使用`--end-of-options`，这样名称参数就不会被误认为是另一个选项。

例如，从shell变量中$REV的修订打印提交对象名称：

```shell
git rev-parse --verify --end-of-options $REV^{commit}
```

如果$REV为空或不是有效修订，这将出错。

与上面类似的：

```shell
git rev-parse --default master --verify --end-of-options $REV
```

但如果$REV为空，将打印来自master的提交对象名称。

### --short[=length]

与[`--verify`](#--verify)相似，但将对象名称缩短为具有至少length个字符的唯一前缀。
最小长度为4，默认为`core.abbrev`配置变量的有效值（参见[`git-config`](git_config.md)）。

参考
----

- [Git - git-rev-parse Documentation](https://git-scm.com/docs/git-rev-parse)
