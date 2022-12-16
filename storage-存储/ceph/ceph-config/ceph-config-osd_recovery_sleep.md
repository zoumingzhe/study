Ceph Config osd_recovery_sleep
==============================

# 相关配置项
 - [osd_recovery_sleep](./ceph-config.md/#osd_recovery_max_active)
 - [osd_recovery_sleep_hdd](./ceph-config.md/#osd_recovery_sleep_hdd)
 - [osd_recovery_sleep_ssd](./ceph-config.md/#osd_recovery_sleep_ssd)

# 代码逻辑
```C++
float OSD::get_osd_recovery_sleep()
{
  if (cct->_conf->osd_recovery_sleep)
    return cct->_conf->osd_recovery_sleep;
  if (!store_is_rotational && !journal_is_rotational)
    return cct->_conf->osd_recovery_sleep_ssd;
  else if (store_is_rotational && !journal_is_rotational)
    return cct->_conf.get_val<double>("osd_recovery_sleep_hybrid");
  else
    return cct->_conf->osd_recovery_sleep_hdd;
}
```

# 判断是否rotational
`cat /sys/block/${blockdevice}/queue/rotational`

# 参考
 * [OSD CONFIG REFERENCE](https://docs.ceph.com/en/latest/rados/configuration/osd-config-ref/)
