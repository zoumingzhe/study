import imp
import mock
import unittest

import sys
sys.modules['unkown_moudle'] = mock.MagicMock()
from unittest_example import B


class test_sys_moudle_mock(unittest.TestCase):

    def setUp(self):
        print('This is setUp')
        print('unkown_moudle', sys.modules['unkown_moudle'])
        print('unkown_moudle.X', sys.modules['unkown_moudle'].X)
        print('unkown_moudle.X.x', sys.modules['unkown_moudle'].X.x)
        print('unkown_moudle.X.x()', sys.modules['unkown_moudle'].X.x())

    def test_sys_moudle_mock(self):
        print('This is test_sys_moudle_mock')
        m = mock.MagicMock()
        print('m', m)
        sys.modules['unkown_moudle'].X.x.return_value = m
        print('unkown_moudle', sys.modules['unkown_moudle'])
        print('unkown_moudle.X', sys.modules['unkown_moudle'].X)
        print('unkown_moudle.X.x', sys.modules['unkown_moudle'].X.x)
        print('unkown_moudle.X.x()', sys.modules['unkown_moudle'].X.x())
        B.b().func()
        B.b().call()


if __name__ == '__main__':
    unittest.main()
