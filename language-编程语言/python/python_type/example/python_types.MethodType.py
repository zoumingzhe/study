import types


def func(args):
    print("function: {}".format(args))


class A():
    def __init__(self):
        self.value = None
    def lock_and_exec_func(self, func, args):
        if isinstance(func, types.MethodType):
            self.value = args
            func(args)
        else:
            print("error")
    def lock_and_exec_name(self, name, args):
        func = getattr(self, name, None)
        if func and isinstance(func, types.MethodType):
            self.value = args
            func(args)
        else:
            print("error")
    def func(self, args):
        print("self.value: {}".format(self.value))
        print("args: {}".format(args))


a = A()
b = A()

a.lock_and_exec_func(a.func, 'a')
a.lock_and_exec_func(b.func, 'b')

a.lock_and_exec_name("func", 'n')
a.lock_and_exec_name("fun1", 'n')

a.lock_and_exec_func(func, 'f')

