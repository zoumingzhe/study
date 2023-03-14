python module ctypes
====================

ctypes 是 Python 的外部函数库。它提供了与 C 兼容的数据类型，并允许调用 DLL 或共享库中的函数。可使用该模块以纯 Python 形式对这些库进行封装。

类型
----

### 基本数据类型

| ctypes type  | C type                                 | Python type              |
| :----------- | :------------------------------------- | :----------------------- |
| c_bool       | _Bool                                  | bool                     |
| c_char       | char                                   | 1-character bytes object |
| c_wchar      | wchar_t                                | 1-character string       |
| c_byte       | char                                   | int                      |
| c_ubyte      | unsigned char                          | int                      |
| c_short      | short                                  | int                      |
| c_ushort     | unsigned short                         | int                      |
| c_int        | int                                    | int                      |
| c_uint       | unsigned int                           | int                      |
| c_long       | long                                   | int                      |
| c_ulong      | unsigned long                          | int                      |
| c_longlong   | __int64 or long long                   | int                      |
| c_ulonglong  | unsigned __int64 or unsigned long long | int                      |
| c_size_t     | size_t                                 | int                      |
| c_ssize_t    | ssize_t or Py_ssize_t                  | int                      |
| c_float      | float                                  | float                    |
| c_double     | double                                 | float                    |
| c_longdouble | long double                            | float                    |
| c_char_p     | char* (NUL terminated)                 | bytes object or `None`   |
| c_wchar_p    | wchar_t* (NUL terminated)              | string or `None`         |
| c_void_p     | void*                                  | int or `None`            |

参考
----

- [ctypes --- Python 的外部函数库](https://docs.python.org/zh-cn/3/library/ctypes.html)
