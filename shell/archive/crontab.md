crontab
=======
Usage:
 crontab [options] file
 crontab [options]
 crontab -n [hostname]

Options:
 -u <user>  define user
 -e         edit user's crontab
 -l         list user's crontab
 -r         delete user's crontab
 -i         prompt before deleting
 -n <host>  set host in cluster to run users' crontabs
 -c         get host in cluster to run users' crontabs
 -s         selinux context
 -x <mask>  enable debugging

# 时间格式
```
*    *    *    *    *
-    -    -    -    -
|    |    |    |    |
|    |    |    |    +----- 星期中星期几 (0 - 6) (星期天 为0)
|    |    |    +---------- 月份 (1 - 12) 
|    |    +--------------- 一个月中的第几天 (1 - 31)
|    +-------------------- 小时 (0 - 23)
+------------------------- 分钟 (0 - 59)

* 取值范围内的所有数字
/ 每过多少个数字
- 从X到Z
，散列数字
```

# 目录
## 任务目录及文件
 - **/var/spool/cron/**：目录下存放每个用户的crontab任务。
 - **/etc/crontab**：此文件存放需要调度的crontab任务，有`user`字段。
 - **/etc/cron.d/**：目录下存放任何需要调度的任务文件，有`user`字段。
 - **/etc/cron.hourly**：目录下存放每小时执行一次的脚本。
 - **/etc/cron.daily**：目录下每天执行一次的脚本。
 - **/etc/cron.weekly**：目录下每周执行一次的脚本。
 - **/etc/cron.monthly**：目录下每月执行一次的脚本。

*任何带扩展名的文件都会被忽略，因此请确保文件没有任何扩展名。*

## 日志目录及文件
 - **Centos7**：`/var/log/cron`
 - **Ubuntu**：`/var/log/cron.log`

若日志文件不存在，开启cron日志：
```shell
# 修改rsyslog
sudo vim /etc/rsyslog.d/50-default.conf
# 去掉cron前面的注释符
#cron.*             /var/log/cron.log
#重启rsyslog
sudo service rsyslog restart
```

# 注意
## 执行时间
新创建的crontab任务并不会立即执行，任务的最小时间间隔是1分钟。

## 执行权限
脚本需要执行权限，否则不会执行。
`chmod +x <file>`

## 环境变量
crontab不提供所执行用户的环境变量，需要在脚本中加入以下内容：
```shell
#!/bin/sh
. /etc/profile
. ~/.bash_profile
```

## 绝对路径
crontab命令中尽量使用绝对路径

# 测试
## /etc/cron.d/
```shell
SHELL=/bin/bash
PATH=/usr/local/sbin:/usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bin

* * * * * root echo "`date "+%Y-%m-%d %H:%M:%S"` cron.d" >> /root/cron.test.txt
```

## /etc/cron.hourly/
```shell
#!/bin/bash

function log
{
  echo -e "`date "+%Y-%m-%d %H:%M:%S"` $1"
}

log "cron.hourly" >> /root/cron.test.txt
```

## /etc/cron.daily/
```shell
#!/bin/bash

function log
{
  echo -e "`date "+%Y-%m-%d %H:%M:%S"` $1"
}

log "cron.daily" >> /root/cron.test.txt
```

# 参考
 * [Linux crontab 命令](https://www.runoob.com/linux/linux-comm-crontab.html)
 * [Linux Crontab 定时任务](https://www.runoob.com/w3cnote/linux-crontab-tasks.html)
