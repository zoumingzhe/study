# m叉搜索树（m-WAY Search Tree）
m-way搜索树是多路树，它是广义的二叉树，其中每个节点包含多个元素。


m-way搜索树可以是空树。如果不是空树，则必须满足以下属性：
 - m阶m-way搜索树，每个节点最多包含`m–1`个（1至`m-1`）元素和`m`个子节点。


 - 在相应的扩展搜索树（用外部节点替换空指针获得）中，每个内部节点最多有m个子节点，在1和m-1元素之间。
 - 每个具有p元素的节点正好有p+1子节点。
 - 假设任何节点都有p个元素，那么C0 - Cp是他们对应的p+1个子元素。

# 数据结构
```C++
struct node {
    int count;
    int value[MAX];
    struct node* child[MAX + 1];
};
```
 - count：节点拥有的元素数
 - value数组：存储节点元素
 - child数组：存储子节点的地址
 - MAX宏：节点可以包含元素的最大值

# 参考
 - [m-WAY Search Trees | Set-1 ( Searching )](https://www.geeksforgeeks.org/m-way-search-trees-set-1-searching)
 - [m-Way Search Tree | Set-2 | Insertion and Deletion](https://www.geeksforgeeks.org/m-way-search-tree-set-2-insertion-and-deletion)
 - [数据结构-第四章 搜索树](https://blog.csdn.net/qq_44202160/article/details/122996148)
