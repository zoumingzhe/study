blkdiscard
==========

```shell
Usage:
 blkdiscard [options] <device>

Discard the content of sectors on a device.

Options:
 -o, --offset <num>  offset in bytes to discard from
 -l, --length <num>  length of bytes to discard from the offset
 -p, --step <num>    size of the discard iterations within the offset
 -s, --secure        perform secure discard
 -z, --zeroout       zero-fill rather than discard
 -v, --verbose       print aligned length and offset

 -h, --help     display this help and exit
 -V, --version  output version information and exit
```

# 参考
 * [blkdiscard(8) - Linux manual page](https://man7.org/linux/man-pages/man8/blkdiscard.8.html)
