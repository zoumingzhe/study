Ceph Config osd_recovery_max_active
===================================

# 相关配置项
 - [osd_recovery_max_active](./ceph-config.md/#osd_recovery_max_active)
 - [osd_recovery_max_active_hdd](./ceph-config.md/#osd_recovery_max_active_hdd)
 - [osd_recovery_max_active_ssd](./ceph-config.md/#osd_recovery_max_active_ssd)

# 代码逻辑
```C++
int OSD::get_recovery_max_active()
{
  if (cct->_conf->osd_recovery_max_active)
    return cct->_conf->osd_recovery_max_active;
  if (store_is_rotational)
    return cct->_conf->osd_recovery_max_active_hdd;
  else
    return cct->_conf->osd_recovery_max_active_ssd;
}
```

# 判断是否rotational
`cat /sys/block/${blockdevice}/queue/rotational`

# 参考
 * [OSD CONFIG REFERENCE](https://docs.ceph.com/en/latest/rados/configuration/osd-config-ref/)
