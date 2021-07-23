# /dev/null
/dev/null空设备是一个特殊的伪文件文件设备，写入到/dev/null的内容都会被丢弃（但报告写入操作成功），读取/dev/null则会立即得到一个EOF。

# 使用/dev/null禁止输出
## 1>/dev/null
使用`cat $filename`会输出filename对应的文件内容（输出到标准输出）,而使用`cat $filename >/dev/null`则不会得到任何信息。

因为，标准输出被重定向到了/dev/null中，使用`cat $filename 1>/dev/null`也会得到同样的效果，因为默认重定向的1就是标准输出。

## 2>/dev/null
使用`cat $filename`时如果filename对应的文件不存在，系统将会报错：“cat: filename: 没有那个文件或目录” 。

如果不想看到错误输出呢，可以使用`cat $filename 2>/dev/null`禁止输出标准错误。

# 获取命令退出码
如果只想确定命令运行是否正常，而不想有任何（标准或错误）输出，可以使用`cat $filename &>/dev/null`或`cat $filename 2>/dev/null >/dev/null`，并使用`echo $?`查看命令的退出码（0正常，1-255出错）。

# 清空文件内容
当需要清空一些文件的内容而不删除文件本身（比如清空日志文件）时，可以使用`cat /dev/null >$filename`或`: >$filename`。

# 参考
 * []()
