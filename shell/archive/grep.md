grep
====
Usage: grep [OPTION]... PATTERN [FILE]...
Search for PATTERN in each FILE.

grep命令用于查找包含指定PATTERN的内容并显示。

# 匹配多个PATTERN
`grep -E "xxx|XXX"`
## 样例
 - `echo -e "123\n456\n789\n"|grep -E "2|4|8"`

# 匹配Perl的正则表达式并输出匹配部分
`grep -Po "xxx"`
## 样例
 - `echo -e "abc123ABC"|grep -Po "\d+"`
 - `echo -e "abc123ABC"|grep -Po "\D+"`
 - `echo -e "abc123ABC"|grep -Po "\S+"`
 - `echo -e "abc123ABC"|grep -Po "[a-z]+"`
 - `echo -e "abc123ABC"|grep -Po "[A-Z]+"`
 - `echo -e "abc123ABC"|grep -Po "[a-zA-Z]+"`

# 输出不匹配行
`grep -v "xxx"`

# 搜索当前目录下所有文件
`grep -rn xxx`

# 参考
 * [Linux grep 命令](https://www.runoob.com/linux/linux-comm-grep.html)
