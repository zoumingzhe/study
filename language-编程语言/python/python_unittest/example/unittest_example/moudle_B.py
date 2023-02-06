
from unkown_moudle import class_x

print('import B')

class class_b():

    def func(self, *args):
        print('This is moudle_B.class_b.func')

    def call(self, *args):
        print('This is moudle_B.class_b.call')
        import sys
        print('unkown_moudle', sys.modules['unkown_moudle'])
        print('unkown_moudle.class_x', sys.modules['unkown_moudle'].class_x)
        print('unkown_moudle.class_x.func()', sys.modules['unkown_moudle'].class_x.func())
        print('class_x', class_x)
        print('class_x.func()', class_x.func())
