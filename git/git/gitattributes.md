gitattributes
=============
定义每个路径的属性。

`.gitattributes`文件是一个简单的文本文件，它为路径提供属性。`.gitattributes`文件中的每一行格式为：
```
pattern attr1 attr2 ...
```
也就是说：
 - 一个模式后跟一个属性列表，用空格分隔。
 - 前导和尾随空格将被忽略。
 - 以#开头的行将被忽略。
 - 以双引号开头的模式以C风格引用。
 - 当模式与路径匹配时，该行中列出的属性将被赋予该路径。


# 参考
 * [Git - gitattributes Documentation](https://git-scm.com/docs/gitattributes)
