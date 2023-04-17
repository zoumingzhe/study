python library os
=================

本模块提供了一种使用与操作系统相关的功能的便捷式途径。 如果你只是想读写一个文件，请参阅 `open()`，如果你想操作文件路径，请参阅 `os.path` 模块，如果你想读取通过命令行给出的所有文件中的所有行，请参阅 `fileinput` 模块。 为了创建临时文件和目录，请参阅 `tempfile` 模块，对于高级文件和目录处理，请参阅 `shutil` 模块。

os.system
---------

os.popen
--------

```python
# VxWorks has no user space shell provided. As a result, running
# command in a shell can't be supported.
if sys.platform != 'vxworks':
    # Supply os.popen()
    def popen(cmd, mode="r", buffering=-1):
        if not isinstance(cmd, str):
            raise TypeError("invalid cmd type (%s, expected string)" % type(cmd))
        if mode not in ("r", "w"):
            raise ValueError("invalid mode %r" % mode)
        if buffering == 0 or buffering is None:
            raise ValueError("popen() does not support unbuffered streams")
        import subprocess, io
        if mode == "r":
            proc = subprocess.Popen(cmd,
                                    shell=True, text=True,
                                    stdout=subprocess.PIPE,
                                    bufsize=buffering)
            return _wrap_close(proc.stdout, proc)
        else:
            proc = subprocess.Popen(cmd,
                                    shell=True, text=True,
                                    stdin=subprocess.PIPE,
                                    bufsize=buffering)
            return _wrap_close(proc.stdin, proc)

    # Helper for popen() -- a proxy for a file whose close waits for the process
    class _wrap_close:
        def __init__(self, stream, proc):
            self._stream = stream
            self._proc = proc
        def close(self):
            self._stream.close()
            returncode = self._proc.wait()
            if returncode == 0:
                return None
            if name == 'nt':
                return returncode
            else:
                return returncode << 8  # Shift left to match old behavior
        def __enter__(self):
            return self
        def __exit__(self, *args):
            self.close()
        def __getattr__(self, name):
            return getattr(self._stream, name)
        def __iter__(self):
            return iter(self._stream)

    __all__.append("popen")
```

参考
----

- [os --- 多种操作系统接口](https://docs.python.org/zh-cn/3/library/os.html)
