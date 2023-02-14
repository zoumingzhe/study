[fio](http://freshmeat.sourceforge.net/projects/fio)
====================================================
fio，又称为Flexible IO Tester，是Jens Axboe（Linux Kernel中block IO subsystem的维护者）编写的用来对硬件进行I/O压力测试和验证工具，支持19种不同的I/O引擎（sync、mmap、libaio、posixaio、SG v3、splice、null、network、syslet、guasi、solarisaio等），I/O priorities(for newer Linux kernels)，rate I/O，forked or threaded jobs等。

# fio安装
 - Linux操作系统
```shell
wget https://brick.kernel.dk/snaps/fio-2.1.10.tar.gz
tar -zxvf fio-2.1.10.tar.gz
cd fio-2.1.10/
./configure
sudo make
sudo make install
```

 - Ubuntu操作系统
```shell
sudo apt install fio
```

 - CentOS操作系统
```shell
yum -y install fio
```

# fio命令解析
fio [options] [job options] <job file(s)>

## fio job file
ini文件格式

## fio readwrite mode
 - readwrite=str,rw=str
    read            顺序读
    write           顺序写
    randwrite       随机写
    randread        随机读
    rw,readwrite    顺序混合读写
    randrw          随机混合读写
 - blocksize=int,bs=int
 - size=int
 - overwrite=bool
 - rwmixread=int
 - rwmixwrite=int

## fio IO engine
`fio --enghelp`
 - ioengine=str

## fio IO depth
 - iodepth=int

## fio IO type
 - direct=bool
 - buffered=bool

## fio num threads
 - numjobs=int

## fio sync
 - sync=bool
 - fsync=int
 - fdatasync=int
 - create_fsync=bool
 - fsync_on_close=bool
 - end_fsync=bool

## fio log
 - write_bw_log=str
 - write_iops_log=str
 - write_lat_log=str
 - log_avg_msec=int

## fio think
 - thinktime=int
 - thinktime_spin=int
 - thinktime_blocks-int

## fio rate
 - rate=int
 - ratemin=int
 - rate_iops=int
 - rate_iops_min=int
 - ratecycle=int

# fio log file formats

# 参考
 * [fio](http://freshmeat.sourceforge.net/projects/fio)
 * [fio I/O测试工具](https://www.oschina.net/p/fio)
 * [fio使用指南（最全的参数说明）](https://blog.csdn.net/sch0120/article/details/76154205)
 * [fio(1) - Linux man page](https://linux.die.net/man/1/fio)
 * [Welcome to FIO’s documentation!](https://fio.readthedocs.io/en/latest/)
