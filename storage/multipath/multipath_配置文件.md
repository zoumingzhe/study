multipath 配置文件
==================

# 配置文件默认设置
`/etc/multipath.conf`配置文件包括`defaults`部分。

```shell
#defaults {
#       polling_interval        10
#       path_selector           "round-robin 0"
#       path_grouping_policy    multibus
#       uid_attribute           ID_SERIAL
#       prio                    alua
#       path_checker            readsector0
#       rr_min_io               100
#       max_fds                 8192
#       rr_weight               priorities
#       failback                immediate
#       no_path_retry           fail
#       user_friendly_names     yes
#}
```

multipath 会使用这些值，除非该属性被`multipath.conf`文件的`devices`和`multipaths`部分所指定的属性覆盖。


# 参考
 * [第 4 章 DM MULTIPATH 配置文件](https://access.redhat.com/documentation/zh-cn/red_hat_enterprise_linux/7/html/dm_multipath/mpio_configfile)
