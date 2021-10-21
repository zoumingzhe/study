# NTP
网络时间协议（Network Time Protocol，NTP）是在数据网络潜伏时间可变的计算机系统之间通过分组交换进行时钟同步的一个网络协议，位于OSI模型的应用层。

NTP的目的是在无序的Internet环境中提供精确和健壮的时间服务，意图将所有参与计算机的协调世界时（UTC）时间同步到几毫秒的误差内。

NTP由特拉华大学的David L. Mills设计，使用Marzullo算法的修改版来选择准确的时间服务器，旨在减轻可变网络延迟造成的影响

NTP通常可以在公共互联网保持几十毫秒的误差，并且在理想的局域网环境中可以实现超过1毫秒的精度，不对称路由和拥塞控制可能导致100毫秒（或更高）的错误。

NTP协议是基于UDP端口号123进行传输。

# SNTP
简单网络时间协议（Simple Network Time Protocol，SNTP）由NTP改编而来。

SNTP简化了NTP的许多算法和步骤,得到了效率,但时间的精度不如NTP。

# 软件时钟和硬件时钟
- 软件时钟：由 Linux 操作系统根据 1970/01/01 开始计算的总秒数；
- 硬件时钟：主机硬件系统上面的时钟，例如 BIOS 记录的时间；

# 参考
1.[网络时间协议](https://zh.wikipedia.org/wiki/%E7%B6%B2%E8%B7%AF%E6%99%82%E9%96%93%E5%8D%94%E5%AE%9A)

2.[第十五章、时间服务器： NTP 服务器](http://cn.linux.vbird.org/linux_server/0440ntp.php)

3.[rfc1095](https://www.rfc-editor.org/rfc/pdfrfc/rfc1095.txt.pdf)

4.[rfc1119](https://www.rfc-editor.org/rfc/pdfrfc/rfc1119.txt.pdf)

5.[rfc1305](https://www.rfc-editor.org/rfc/pdfrfc/rfc1305.txt.pdf)

6.[rfc2030](https://www.rfc-editor.org/rfc/pdfrfc/rfc2030.txt.pdf)

7.[rfc5905](https://www.rfc-editor.org/rfc/pdfrfc/rfc5905.txt.pdf)

8.[rfc5906](https://www.rfc-editor.org/rfc/pdfrfc/rfc5906.txt.pdf)
