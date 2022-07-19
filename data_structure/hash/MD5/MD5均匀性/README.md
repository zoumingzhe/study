MD5均匀性
==========
一个合格的散列函数应当包含三个特征：
- **单向性**：容易计算输入的散列结果，但是从散列结果无法推出输入内容；
- **抗碰撞性**：很难找到两个不同的输入内容，得到相同的输出结果；
- **映射均匀性和差分均匀性**：散列结果中 bit 位上的 0 的数量和 1 的数量应当大致相等；改变输入内容的1 个 bit 信息会导致散列结果一半以上的 bit 位变化（雪崩效应）。

将MD5的散列结果以十六进制的形式表示，那么结果则是1234567890abcdef这十六个可能的结果组合。如果十六个可能的结果是等概率（均匀）的，那么对于某个十六进制位来说，它的信息熵为4。

# 测试
在Python3上运行测试代码[测试代码](./MD5均匀性分析.py)。

# 结论
MD5 的散列结果是（接近）均匀的。

# 参考
 * [MD5 散列函数的结果是均匀分布吗？](https://liam.page/2015/08/12/does-the-result-of-md5-hash-function-uniform/)
 * [信息熵](https://baike.baidu.com/item/信息熵)
 * [NameError:name ‘xrange’ is not defined](https://www.cnblogs.com/hdk1993/p/8893991.html)
 * [md5加密报错解决方法（TypeError: Unicode-objects must be encoded before hashing）](https://www.cnblogs.com/cheng10/p/9995668.html)
