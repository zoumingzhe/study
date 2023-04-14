python unittest
===============

unittest测试用例三部曲：建立测试场景（模拟）、执行被测代码（调用）、与预期结果比较（断言）

unittest.mock
-------------

mock是一个用于在Python中进行测试的库。它允许用模拟对象替换被测系统的某些部分，并对它们的使用方式做出断言。

### patchers

#### patch

```python
unittest.mock.patch(target, new=DEFAULT, spec=None, create=False, spec_set=None, autospec=None, new_callable=None, **kwargs)
```

mock提供了一个patch()装饰器，充当函数装饰器、类装饰器或上下文管理器。

在测试范围内（函数体或with语句中），target被一个模拟对象替换。当函数或with语句退出时，替换被撤销。

target应该是形式为`package.module.ClassName`的字符串。

#### patch.object

```python
patch.object(target, attribute, new=DEFAULT, spec=None, create=False, spec_set=None, autospec=None, new_callable=None, **kwargs)¶
```

使用模拟对象替换对象（target）上的有名成员（attribute）。

##### patch和patch.object的区别

 1. 用法上的区别：patch使用一个字符串描述需要替换的target，不需要导入target；patch.object则需要导入target，替换的是target的成员。
 2. 本质上的区别：patch是通过修改sys.modules，在替换的对象在实体化时将被实例化成一个mock对象；patch.object则是直接替换已存在对象的成员，但并不能替换已存在的对象。

##### 被测代码中import module，模拟module.class

直接使用patch替换`package.module.ClassName`即可，在class实例化时将被实例化成一个mock对象。

##### 被测代码中import class，模拟已被导入的class

由于class已经被导入，patch替换`package.module.ClassName`的将是sys.modules，而不是已经导入的对象。

此时，可以通过`patch.object(tested_object, imported_object)`将被测代码中已导入的class对象替换成mock对象。

#### patch.dict

```python
patch.dict(in_dict, values=(), clear=False, **kwargs)
```

##### 替换dict中旧的key

```python
d = {'old_key': 'old_value'}
print("before : {}".format(d))
with mock.patch.dict(d, {'old_key': 'new_value'}):
    print("{}".format(d)) # {'old_key': 'new_value'}
print("after : {}".format(d))
```

##### 在dict中增加新的key

```python
d = {'old_key': 'old_value'}
print("before : {}".format(d))
with mock.patch.dict(d, {'new_key': 'new_value'}):
    print("{}".format(d)) # {'old_key': 'old_value', 'new_key': 'new_value'}
print("after : {}".format(d))
```

##### 清空dict并替换（clean = True）

```python
d = {'old_key': 'old_value'}
print("before : {}".format(d))
with mock.patch.dict(d, {'new_key': 'new_value'}, True):
    print("{}".format(d)) # {'new_key': 'new_value'}
print("after : {}".format(d))
```

#### 模拟属性（property）

property()函数的在新式类中返回属性值，可设置get、set、del方法和doc。
访问类的属性时触发getter，对属性赋值时触发setter，del时触发deleter。

模拟属性时通过`mock.MagicMock(spec = ['a', 'b'])`指定属性列表，然后通过`<mock_object>.a =`对属性直接赋值，即可通过`<mock_object>.a`获取属性值。

##### PropertyMock 模拟类的属性

PropertyMock提供__get__()和__set__()方法，旨在用作类的属性或其他描述符的模拟。

```python
c = mock.MagicMock()
type(c).p = mock.PropertyMock(side_effect = [1,2,3])
print(c.p) # 1
print(c.p) # 2
print(c.p) # 3
```

#### 模拟方法

模拟属性时通过`mock.MagicMock(spec = ['a', 'b'])`指定方法列表，然后通过`<mock_object>.a.return_value =`或者`<mock_object>.a.side_effect =`模拟调用，即可通过`<mock_object>.a()`调用方法。

示例：

```
<mock_object>.a.return_value = True
<mock_object>.a.return_value = mock.MagicMock()
<mock_object>.a.side_effect = function()
<mock_object>.a.side_effect = IOError('UT')
<mock_object>.a.side_effect = [True, mock.MagicMock(), function(), IOError('UT')]
```

##### return_value和side_effect的区别

 1. **side_effect会覆盖return_value。**
 2. return_value返回固定值，没有调用次数限制；side_effect是一个迭代器，根据指定的迭代列表动态返回。
 3. 当side_effect的迭代列表为空后，后续调用将触发异常，但是可以为side_effect继续指定迭代列表。

#### 模拟异常

模拟属性时通过`mock.MagicMock(spec = ['a', 'b'])`指定方法列表，然后通过`<mock_object>.a.side_effect =`模拟异常，即可在`<mock_object>.a()`调用方法时触发异常。

##### 模拟自定义异常

Python中，通过创建一个新的异常类（直接或间接的继承Exception类），可以自定义异常。

在模拟异常时，对于标准异常，可以直接访问。但是，对于自定义异常，则可能无法直接访问（例如：定义了自定义异常的module被mock，则自定义异常也成为了一个mock对象，而不是继承了Exception类的异常类），需要对自定义异常进行模拟。

模拟自定义异常的方法是，自定义一个模拟的异常类并替换原有的异常类，示例如下：

```python
class MockException(Exception):
    
    def __init__(self, *args, **kwargs):
        pass

class MockNotFoundException(MockException):
    """Used as mock for module.NotFound."""

class test_module(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        module.NotFound = MocNotFoundException
```

### MagicMock

### NonCallableMock

### AsyncMock

assert
------

### unittest断言

使用self.assert...形式断言，通常用以验证返回值。

for python2:
| Method                    | Checks that          | New in |
| ------------------------- | -------------------- | ------ |
| assertEqual(a, b)         | a == b               |        |
| assertNotEqual(a, b)      | a != b               |        |
| assertTrue(x)             | bool(x) is True      |        |
| assertFalse(x)            | bool(x) is False     |        |
| assertIs(a, b)            | a is b               | 2.7    |
| assertIsNot(a, b)         | a is not b           | 2.7    |
| assertIsNone(x)           | x is None            | 2.7    |
| assertIsNotNone(x)        | x is not None        | 2.7    |
| assertIn(a, b)            | a in b               | 2.7    |
| assertNotIn(a, b)         | a not in b           | 2.7    |
| assertIsInstance(a, b)    | isinstance(a, b)     | 2.7    |
| assertNotIsInstance(a, b) | not isinstance(a, b) | 2.7    |

for python3:
| Method                    | Checks that          | New in |
| ------------------------- | -------------------- | ------ |
| assertEqual(a, b)         | a == b               |        |
| assertNotEqual(a, b)      | a != b               |        |
| assertTrue(x)             | bool(x) is True      |        |
| assertFalse(x)            | bool(x) is False     |        |
| assertIs(a, b)            | a is b               | 3.1    |
| assertIsNot(a, b)         | a is not b           | 3.1    |
| assertIsNone(x)           | x is None            | 3.1    |
| assertIsNotNone(x)        | x is not None        | 3.1    |
| assertIn(a, b)            | a in b               | 3.1    |
| assertNotIn(a, b)         | a not in b           | 3.1    |
| assertIsInstance(a, b)    | isinstance(a, b)     | 3.2    |
| assertNotIsInstance(a, b) | not isinstance(a, b) | 3.2    |

说明：

 1. isinstance(obj, cls)用于判断object是否为class的实例
 2. 尽量不要用使用assertTrue，因为除None、False、0以外的任何值均为True（包括字符串、对象、异常）
 3. 使用assertEqual严格判定类型

#### 断言异常

如果调用触发异常，如何断言？

for python2:
| Method                                         | Checks that                                                   | New in |
| ---------------------------------------------- | ------------------------------------------------------------- | ------ |
| assertRaises(exc, fun, *args, **kwds)          | fun(*args, **kwds) raises exc                                 |        |
| assertRaisesRegexp(exc, r, fun, *args, **kwds) | fun(*args, **kwds) raises exc and the message matches regex r | 2.7    |

for python3:
| Method                                        | Checks that                                                    | New in |
| --------------------------------------------- | -------------------------------------------------------------- | ------ |
| assertRaises(exc, fun, *args, **kwds)         | fun(*args, **kwds) raises exc                                  |        |
| assertRaisesRegex(exc, r, fun, *args, **kwds) | fun(*args, **kwds) raises exc and the message matches regex r  | 3.1    |
| assertWarns(warn, fun, *args, **kwds)         | fun(*args, **kwds) raises warn                                 | 3.2    |
| assertWarnsRegex(warn, r, fun, *args, **kwds) | fun(*args, **kwds) raises warn and the message matches regex r | 3.2    |
| assertLogs(logger, level)                     | The with block logs on logger with minimum level               | 3.4    |
| assertNoLogs(logger, level)                   | The with block does not log on logger with minimum level       | 3.10   |

### mock断言

使用<mock_object>.asset...形式断言，通常用以验证程序中间调用。

- assert_called()
    断言模拟至少被调用一次。
- assert_called_once()
    断言模拟只被调用了一次。
- assert_called_with(*args, **kwargs)
    断言模拟最后一次调用使用指定的参数。
- assert_called_once_with(*args, **kwargs)
    断言模拟只被调用了一次，并且该调用使用指定的参数。
- assert_any_call(*args, **kwargs)
    断言模拟已经使用指定的参数调用了模拟。
- assert_has_calls(calls, any_order=False)
    断言模拟已经使用指定的调用列表（calls）调用了模拟。
- assert_not_called()
    断言模拟从未被调用。

参考
----

- [unittest — Unit testing framework](https://docs.python.org/3/library/unittest.html#assert-methods)
- [unittest.mock — mock object library](https://docs.python.org/3/library/unittest.mock.html#unittest)
- [Python 异常处理](https://www.runoob.com/python/python-exceptions.html)
