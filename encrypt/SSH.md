# SSH（安全外壳协议）
SSH为Secure Shell的缩写，由IETF的网络小组（Network Working Group）所制定。

SSH为建立在应用层基础上的安全协议，专为远程登录会话和其他网络服务提供安全性的协议。利用SSH协议可以有效防止远程管理过程中的信息泄露问题，SSH在正确使用时可弥补网络中的漏洞。

SSH最初是UNIX系统上的一个程序，后来又迅速扩展到其他操作平台。SSH客户端适用于多种平台，几乎所有UNIX平台（包括HP-UX、Linux、AIX、Solaris、Digital UNIX、Irix）以及其他平台，都可运行SSH。

# OpenSSH
OpenSSH是SSH（Secure SHell）协议的免费开源实现。

SSH协议族可以用来进行远程控制，或在计算机之间传送文件。而实现此功能的传统方式，如telnet（终端仿真协议）、rcp ftp、rlogin、rsh都是极为不安全的，并且会使用明文传送密码。OpenSSH提供了服务端后台程序和客户端工具，用来加密远程控制和文件传输过程中的数据，并由此来代替原来的类似服务。

# SSH操作
## 获取密钥
 - ssh-keygen -t [rsa|dsa] : 生成私钥（id_rsa|id_dsa）与公钥（id_rsa.pub|id_dsa.pub）对
 - cat ~/.ssh/（id_rsa.pub|id_dsa.pub） : 查看本机当前用户的公钥（id_rsa.pub|id_dsa.pub）
## 免密登陆
 - ssh-keygen -t [rsa|dsa] : 生成私钥（id_rsa|id_dsa）与公钥（id_rsa.pub|id_dsa.pub）对
 - ssh-copy-id [user@]hostname : 将本机当前用户的公钥（id_rsa.pub|id_dsa.pub）复制到远程主机（~/.ssh/authorized_keys）中，该命令执行需要输入远程主机user的password
 - cat ~/.ssh/（id_rsa.pub|id_dsa.pub） : 查看本机当前用户的公钥（id_rsa.pub|id_dsa.pub）并发送给远程主机的user，让远程主机的user将本机当前用户的公钥（id_rsa.pub|id_dsa.pub）写入（~/.ssh/authorized_keys）中

# 参考
1.[SSH](https://baike.baidu.com/item/ssh/10407)

2.[OpenSSH](https://baike.baidu.com/item/OpenSSH)
