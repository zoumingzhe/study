import imp
import mock
import unittest

import sys
sys.modules['unkown_moudle'] = mock.MagicMock()
from unkown_moudle import X
from unittest_example import B


class test_moudle_mock_patch_object(unittest.TestCase):

    def setUp(self):
        print('This is setUp')
        print('unkown_moudle', sys.modules['unkown_moudle'])
        print('unkown_moudle.X', sys.modules['unkown_moudle'].X)
        print('unkown_moudle.X.x', sys.modules['unkown_moudle'].X.x)
        print('unkown_moudle.X.x()', sys.modules['unkown_moudle'].X.x())

    @mock.patch.object(X, 'x')
    def test_moudle_mock_patch_object(self, mock_x):
        print('This is test_moudle_mock_patch_object')
        m = mock.MagicMock()
        print('m', m)
        mock_x.return_value = m
        print('mock_x', mock_x)
        print('unkown_moudle', sys.modules['unkown_moudle'])
        print('unkown_moudle.X', sys.modules['unkown_moudle'].X)
        print('unkown_moudle.X.x', sys.modules['unkown_moudle'].X.x)
        print('unkown_moudle.X.x()', sys.modules['unkown_moudle'].X.x())
        B.b().func()
        B.b().call()


if __name__ == '__main__':
    unittest.main()
