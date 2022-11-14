blockdev
========
```shell
Usage:
 blockdev -V
 blockdev --report [devices]
 blockdev [-v|-q] commands devices

Available commands:
 --getsz                   get size in 512-byte sectors
 --setro                   set read-only
 --setrw                   set read-write
 --getro                   get read-only
 --getdiscardzeroes        get discard zeroes support status
 --getss                   get logical block (sector) size
 --getpbsz                 get physical block (sector) size
 --getiomin                get minimum I/O size
 --getioopt                get optimal I/O size
 --getalignoff             get alignment offset in bytes
 --getmaxsect              get max sectors per request
 --getbsz                  get blocksize
 --setbsz <bytes>          set blocksize on file descriptor opening the block device
 --getsize                 get 32-bit sector count (deprecated, use --getsz)
 --getsize64               get size in bytes
 --setra <sectors>         set readahead
 --getra                   get readahead
 --setfra <sectors>        set filesystem readahead
 --getfra                  get filesystem readahead
 --flushbufs               flush buffers
 --rereadpt                reread partition table
```

# 参考
 * [blockdev(8) - Linux manual page](https://man7.org/linux/man-pages/man8/blockdev.8.html)
