partprobe
=========

```text
Usage: partprobe [OPTION] [DEVICE]...
Inform the operating system about partition table changes.

  -d, --dry-run    do not actually inform the operating system
  -s, --summary    print a summary of contents
  -h, --help       display this help and exit
  -v, --version    output version information and exit

When no DEVICE is given, probe all partitions.

Report bugs to <bug-parted@gnu.org>.
```

# 参考
 * [partprobe(8) - Linux manual page](https://man7.org/linux/man-pages/man8/partprobe.8.html)
