python PyPA [pip](https://pip.pypa.io/en/stable/)
=================================================

pip is the package installer for Python.

pip [config](https://pip.pypa.io/en/stable/topics/configuration/)
-----------------------------------------------------------------

```config
[global]
timeout = 60
index-url = http://mirrors.aliyun.com/pypi/simple/
trusted-host = mirrors.aliyun.com
```

pip index
---------

如果pip源的下载速度非常慢或者没有待安装的包，则需指定新的pip源：

- 使用`-i <url>`或`--index-url <url>`参数为当前`pip install`命令指定镜像地址
- 通过`pip config set`全局修改`global.index-url`

pip 中国大陆镜像：

|          | url                                        |
| :------- | :----------------------------------------- |
| tsinghua | <https://pypi.tuna.tsinghua.edu.cn/simple> |
| ustc     | <https://pypi.mirrors.ustc.edu.cn/simple>  |
| baidu    | <https://mirror.baidu.com/pypi/simple>     |
| aliyun   | <https://mirrors.aliyun.com/pypi/simple>   |
| douban   | <https://pypi.douban.com/simple>           |

参考
----

- [pip · PyPI](https://pypi.org/project/pip/)
- [pypa/pip: The Python package installer](https://github.com/pypa/pip)
- [Python pip 安装与使用](https://www.runoob.com/w3cnote/python-pip-install-usage.html)
