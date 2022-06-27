# 基数树（Radix Tree）
对于稀疏的Hash表，如何解决Hash冲突和Hash表大小的设计是一个很头疼的问题。

Radix树就是针对这样的稀疏的长整型数据查找，实现高效且节省空间地映射。它存储有效率，并且可快速查询，用于对象ID到对象指针的映射（如：IDR机制）、内存管理等。

Radix树的设计思想来自于DonaldR.Morrison于1968年提出的Patricia树（Practical Algorithm To Retrieve Information Coded In Alphanumeric），这是一种基于二进制表示的键值的查找树，尤其适合处理非常长的、可变长度的键值。

Patricia的基本思想是构建一个二叉树，在每个节点中都存储有在进行下一次bit测试之前需要跳过的bit数目，以此来避免单路分支。Patricia树一般由内部节点和外部节点组成，内部节点指示需要进行bit测试的位置，并根据bit测试结果决定查找操作前进的方向；外部节点用于存储键值，查找操作将于外部节点处终结。

# 原理
Radix树是一种多叉搜索树。树的叶子结点是实际的数据条目。每一个结点有一个固定的、2^n指针指向子结点（每一个指针称为槽slot，n为划分的基的大小）。

# 参考
 - [Radix树](https://baike.baidu.com/item/Radix%E6%A0%91)
 - [查找——图文翔解RadixTree（基数树）](https://www.cnblogs.com/wgwyanfs/p/6887889.html)
