nohup
=====

nohup（no hang up，不挂起）命令用于在系统后台不挂断地运行命令，退出终端不会影响程序的运行。

nohup命令在默认情况下（非重定向时）会输出一个nohup.out文件到当前目录下，如果当前目录的nohup.out文件不可写，输出重定向到$HOME/nohup.out文件中。

```text
Usage: nohup COMMAND [ARG]...
  or:  nohup OPTION
Run COMMAND, ignoring hangup signals.

      --help     display this help and exit
      --version  output version information and exit

If standard input is a terminal, redirect it from an unreadable file.
If standard output is a terminal, append output to 'nohup.out' if possible,
'$HOME/nohup.out' otherwise.
If standard error is a terminal, redirect it to standard output.
To save output to FILE, use 'nohup COMMAND > FILE'.

NOTE: your shell may have its own version of nohup, which usually supersedes
the version described here.  Please refer to your shell's documentation
for details about the options it supports.

GNU coreutils online help: <https://www.gnu.org/software/coreutils/>
Report nohup translation bugs to <https://translationproject.org/team/>
Full documentation at: <https://www.gnu.org/software/coreutils/nohup>
or available locally via: info '(coreutils) nohup invocation'
```

# 参考
 * [Linux nohup 命令](https://www.runoob.com/linux/linux-comm-nohup.html)
