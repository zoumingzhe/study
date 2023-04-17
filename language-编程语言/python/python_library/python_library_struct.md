python library struct
=====================

将字节串解读为打包的二进制数据

格式字符串
----------

### 字节顺序，大小和对齐方式

| Character | Byte order             | Size     | Alignment |
| :-------- | :--------------------- | :------- | :-------- |
| @         | native                 | native   | native    |
| =         | native                 | standard | none      |
| <         | little-endian          | standard | none      |
| >         | big-endian             | standard | none      |
| !         | network (= big-endian) | standard | none      |

如果第一个字符不是其中之一，则假定为 `'@'` 。

本机大小和对齐方式是使用 C 编译器的 `sizeof` 表达式来确定的。 这总是会与本机字节顺序相绑定。

请注意 `'@'` 和 `'='` 之间的区别：两个都使用本机字节顺序，但后者的大小和对齐方式是标准化的。

### 格式字符

| Format | C Type             | Python type       | Standard size | Notes    |
| :----- | ------------------ | ----------------- | ------------- | -------- |
| x      | pad byte           | no value          |               | (7)      |
| c      | char               | bytes of length 1 | 1             |          |
| b      | signed char        | integer           | 1             | (1), (2) |
| B      | unsigned char      | integer           | 1             | (2)      |
| ?      | _Bool              | bool              | 1             | (1)      |
| h      | short              | integer           | 2             | (2)      |
| H      | unsigned short     | integer           | 2             | (2)      |
| i      | int                | integer           | 4             | (2)      |
| I      | unsigned int       | integer           | 4             | (2)      |
| l      | long               | integer           | 4             | (2)      |
| L      | unsigned long      | integer           | 4             | (2)      |
| q      | long long          | integer           | 8             | (2)      |
| Q      | unsigned long long | integer           | 8             | (2)      |
| n      | ssize_t            | integer           |               | (3)      |
| N      | size_t             | integer           |               | (3)      |
| F      | (6)                | float             | 2             | (4)      |
| f      | float              | float             | 4             | (4)      |
| d      | double             | float             | 8             | (4)      |
| s      | char[]             | bytes             |               | (9)      |
| p      | char[]             | bytes             |               | (8)      |
| P      | void*              | integer           |               | (5)      |

*在 3.3 版*更改：增加了对 `'n'` 和 `'N'` 格式的支持
*在 3.6 版*更改：添加了对 `'e'` 格式的支持。

参考
----

- [struct --- 将字节串解读为打包的二进制数据](https://docs.python.org/zh-cn/3/library/struct.html)
