# [fio](http://freshmeat.sourceforge.net/projects/fio)
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
## fio ioengine
`fio --enghelp`

# 参考
 * [fio](http://freshmeat.sourceforge.net/projects/fio)
 * [fio I/O测试工具](https://www.oschina.net/p/fio)
