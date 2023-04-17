python typing —— 类型注解
=========================

**Python 运行时不强制执行函数和变量类型注解，但这些注解可用于类型检查器、IDE、静态检查器等第三方工具。**

Any
---

`Any`是一种特殊的类型。静态类型检查器认为所有类型均与`Any`兼容，同样，`Any`也与所有类型兼容。

也就是说，可对`Any`类型的值执行任何操作或方法调用，并赋值给任意变量。

此外，未指定返回值与参数类型的函数，都隐式地默认使用Any。

### `Any`和`object`的区别

与`Any`相似，所有类型都是`object`的子类型。然而，与`Any`不同，`object`不可逆：`object`不是其它类型的子类型。

就是说，值的类型是`object`时，类型检查器几乎会拒绝所有对它的操作，并且，把它赋给更精确的类型变量（或返回值）属于类型错误。

NewType
-------

使用`NewType`助手来创建不同的类型：

```Python
from typing import NewType

UserId = NewType('UserId', int)
some_id = UserId(524313)
```

静态类型检查器把新类型当作原始类型的子类，这种方式适用于捕捉逻辑错误：

```Python
def get_user_name(user_id: UserId) -> str:
    ...

# passes type checking
user_a = get_user_name(UserId(42351))

# fails type checking; an int is not a UserId
user_b = get_user_name(-1)
```

`UserId`类型的变量可执行所有`int`操作，但返回结果都是`int`类型。这种方式允许在预期`int`时传入`UserId`，还能防止意外创建无效的`UserId`：

```Python
# 'output' is of type 'int', not 'UserId'
output = UserId(23413) + UserId(54341)
```

***注意，这些检查只由静态类型检查器强制执行。在运行时，语句`Derived = NewType('Derived', Base)`将产生一个`Derived`可调用对象，该对象立即返回你传递给它的任何参数。 这意味着语句`Derived(some_value)`不会创建一个新的类，也不会引入超出常规函数调用的很多开销。***

更确切地说，在运行时，`some_value is Derived(some_value)`表达式总为True。

创建`Derived`的子类型是无效的:

```Python
from typing import NewType

UserId = NewType('UserId', int)

# Fails at runtime and does not pass type checking
class AdminUserId(UserId): pass
```

然而，我们可以在"派生的"`NewType`的基础上创建一个`NewType`。

```Python
from typing import NewType

UserId = NewType('UserId', int)

ProUserId = NewType('ProUserId', UserId)
```

类型别名
--------

把类型赋给别名，就可以定义类型别名。类型别名适用于简化复杂的类型签名。

本例中，`Vector`和`list[float]`相同，可互换：

```Python
Vector = list[float]

def scale(scalar: float, vector: Vector) -> Vector:
    return [scalar * num for num in vector]

# passes type checking; a list of floats qualifies as a Vector.
new_vector = scale(2.0, [1.0, -4.2, 5.4])
```

可调对象（Callable）
------------------

预期特定签名回调函数的框架可以用`Callable[[Arg1Type, Arg2Type], ReturnType]`实现类型提示。

例如：

```Python
from collections.abc import Callable

def feeder(get_next_item: Callable[[], str]) -> None:
    # Body

def async_query(on_success: Callable[[int], None],
                on_error: Callable[[int, Exception], None]) -> None:
    # Body

async def on_update(value: str) -> None:
    # Body
callback: Callable[[str], Awaitable[None]] = on_update
```

无需指定调用签名，用省略号字面量替换类型提示里的参数列表：`Callable[..., ReturnType]`，就可以声明可调对象的返回类型。

以其他可调用对象为参数的可调用对象可以使用`ParamSpec`来表明其参数类型是相互依赖的。此外，如果该可调用对象增加或删除了其他可调用对象的参数，可以使用`Concatenate`操作符。它们分别采取`Callable[ParamSpecVariable, ReturnType]`和`Callable[Concatenate[Arg1Type, Arg2Type, ..., ParamSpecVariable], ReturnType]`的形式。

参考
----

- [typing —— 类型注解支持 — Python 3.11.3 文档](https://docs.python.org/zh-cn/3/library/typing.html)
