python pip
==========

pip is the package installer for Python.

pip config
----------

- Linux:`~/.pip/pip.conf`
- Windows:`~/.pip/pip.conf`

```config
[global]
timeout = 60
index-url = http://mirrors.aliyun.com/pypi/simple/
trusted-host = mirrors.aliyun.com
```

pip 镜像源
----------

pip 中国镜像：

|          | url                                        |
| :------- | :----------------------------------------- |
| tsinghua | <https://pypi.tuna.tsinghua.edu.cn/simple> |
| ustc     | <https://pypi.mirrors.ustc.edu.cn/simple>  |
| baidu    | <https://mirror.baidu.com/pypi/simple>     |
| aliyun   | <https://mirrors.aliyun.com/pypi/simple>   |
| douban   | <https://pypi.douban.com/simple>           |

参考
----

- [pip](https://pip.pypa.io/en/stable/)
- [Python pip 安装与使用](https://www.runoob.com/w3cnote/python-pip-install-usage.html)
