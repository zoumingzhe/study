python iterator —— 迭代器
=========================

任何实现了__iter__()和__next__()方法的对象都是迭代器。__iter__返回一个可迭代对象，__next__返回迭代器的下一个值。

**注意：**next**()方法为Python3中迭代器的定义，Python2中迭代器的定义是next()方法。**

示例1：

```python
class Fib():
    def __init__(self, max):
        self.n = 0
        self.prev = 0
        self.curr = 1
        self.max = max

    def __iter__(self):
        return self

    def __next__(self):
        if self.n < self.max:
            value = self.curr
            self.curr += self.prev
            self.prev = value
            self.n += 1
            return value
        else:
            raise StopIteration

# call
f = Fib(5)
print(next(f)) # 1
print(next(f)) # 1
print(next(f)) # 2
print(next(f)) # 3
print(next(f)) # 5
print(next(f)) # StopIteration
```

示例2：

```python
class Container:
    def __init__(self):
        self.values = {}
    def __getitem__(self, name):
        return self.values[name]
    def __setitem__(self, name, value):
        self.values[name] = value
    def __delitem__(self, name):
        del self.values[name]
    def __iter__(self):
        return iter(self.values)

# call
thing = Container()
thing['one'] = 1
thing['one'] # 1
'one' in thing # True
del thing['one']
'one' in thing # False
```
