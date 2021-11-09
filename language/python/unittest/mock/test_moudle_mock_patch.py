import imp
import mock
import unittest

import sys
sys.modules['unkown_moudle'] = mock.MagicMock()
from unittest_example import B


class test_moudle_mock_patch(unittest.TestCase):

    def setUp(self):
        print('This is setUp')
        print('unkown_moudle', sys.modules['unkown_moudle'])
        print('unkown_moudle.X', sys.modules['unkown_moudle'].X)
        print('unkown_moudle.X.x', sys.modules['unkown_moudle'].X.x)
        print('unkown_moudle.X.x()', sys.modules['unkown_moudle'].X.x())

    @mock.patch('unkown_moudle.X')
    def test_moudle_mock_patch(self, mock_X):
        print('This is test_moudle_mock_patch')
        m = mock.MagicMock()
        print('m', m)
        mock_X.x.return_value = m
        print('mock_X', mock_X)
        print('mock_X.x', mock_X.x)
        print('mock_X.x()', mock_X.x())
        print('unkown_moudle', sys.modules['unkown_moudle'])
        print('unkown_moudle.X', sys.modules['unkown_moudle'].X)
        print('unkown_moudle.X.x', sys.modules['unkown_moudle'].X.x)
        print('unkown_moudle.X.x()', sys.modules['unkown_moudle'].X.x())
        B.b().func()
        B.b().call()


if __name__ == '__main__':
    unittest.main()
