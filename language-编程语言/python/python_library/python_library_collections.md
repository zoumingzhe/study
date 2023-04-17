python library collections
==========================

这个模块实现了特定目标的容器，以提供Python标准内建容器[dict](../python_type/python_dict.md)、[list](../python_type/python_list.md)、[set](../python_type/python_set.md)和[tuple](../python_type/python_tuple.md)的替代选择。

|                           | description                                                   |
| :------------------------ | :------------------------------------------------------------ |
| [namedtuple](#namedtuple) | 创建命名元组子类的工厂函数                                    |
| deque                     | 类似列表(list)的容器，实现了在两端快速添加(append)和弹出(pop) |
| ChainMap                  | 类似字典(dict)的容器类，将多个映射集合到一个视图里面          |
| Counter                   | dict subclass for counting hashable objects                   |
| OrderedDict               | 字典的子类，保存了他们被添加的顺序                            |
| defaultdict               | 字典的子类，提供了一个工厂函数，为字典查询提供一个默认值      |
| UserDict                  | 封装了字典对象，简化了字典子类化                              |
| UserList                  | 封装了列表对象，简化了列表子类化                              |
| UserString                | 封装了字符串对象，简化了字符串子类化                          |

namedtuple
----------

命名元组赋予每个位置一个含义，提供可读性和自文档性。它们可以用于任何普通元组，并添加了通过名字获取值的能力，通过索引值也是可以的。

`collections.namedtuple(typename, field_names, *, rename=False, defaults=None, module=None)`

返回一个新的元组子类，名为`typename`。这个新的子类用于创建类元组的对象，可以通过字段名来获取属性值，同样也可以通过索引和迭代获取值。子类实例同样有文档字符串（类名和字段名）另外一个有用的`__repr__()`方法，以`name=value`格式列明了元组内容。

`field_names`是一个像`[‘x’, ‘y’]`一样的字符串序列。另外`field_names`可以是一个纯字符串，用空白或逗号分隔开元素名，比如`'x y'`或者`'x, y'`。

任何有效的`Python`标识符都可以作为字段名，除了下划线开头的那些。有效标识符由字母，数字，下划线组成，但首字母不能是数字或下划线，另外不能是关键词`keyword`比如`class`,`for`,`return`,`global`,`pass`,或`raise`。

如果`rename`为真，无效字段名会自动转换成位置名。比如`['abc', 'def', 'ghi', 'abc']`转换成`['abc', '_1', 'ghi', '_3']`，消除关键词`def`和重复字段名`abc`。

`defaults`可以为`None`或者是一个默认值的[iterable](../python_iterator.md)。如果一个默认值域必须跟其他没有默认值的域在一起出现，`defaults`就应用到最右边的参数。比如如果域名`['x', 'y', 'z']`和默认值`(1, 2)`，那么`x`就必须指定一个参数值，`y`默认值`1`，`z`默认值`2`。

如果`module`值有定义，命名元组的`__module__`属性值就被设置。

参考
----

- [collections --- 容器数据类型](https://docs.python.org/zh-cn/3/library/collections.html)
