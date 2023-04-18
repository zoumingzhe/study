python package [attrs](https://www.attrs.org/)
==============================================

`attrs`是`Python`包，它将通过将您从实现对象协议（又名特殊方法`dunder methods`或魔法函数`magic methods`）的苦差事中解救出来，带回编写类的乐趣。

它的主要目标是帮助您编写简洁且正确的软件，而不会降低您的代码速度。

[Type Annotations](https://www.attrs.org/en/stable/types.html#)
---------------------------------------------------------------

`attrs`为`Python 3.6`（[PEP 526](https://peps.python.org/pep-0526/)） 和遗留语法提供了对类型注释的一流支持。

类型注释将永远是*可选的*。**但请记住，如果您选择使用类型注释，则必须注释所有属性！**

### Type annotation and type argument cannot both be present

类型注释和[类型注解](../python_library/python_library_typing.md)不能同时存在。

推荐在`Python3`中使用`类型注解`，因为这是[Python3 文档](https://docs.python.org/zh-cn/3/library/typing.html)中介绍的用法，并且`类型注解`可用于类型检查器、IDE、静态检查器等第三方工具。

参考
----

- [python-attrs/attrs: Python Classes Without Boilerplate](https://github.com/python-attrs/attrs)
- [Best Practice in Python 3 for Type Annotation? · Issue #582 · python-attrs/attrs](https://github.com/python-attrs/attrs/issues/582)
