Ceph PLACEMENT GROUP CONCEPTS （归置组概念）
===========================================

## Acting Set （在任集合）
```
The ordered list of OSDs who are (or were as of some epoch) responsible for a particular placement group.
```

负责特定归置组的当前（或者历史版本）OSD有序列表。

## Up Set （当选集合）
```
The ordered list of OSDs responsible for a particular placement group for a particular epoch according to CRUSH. Normally this is the same as the Acting Set, except when the Acting Set has been explicitly overridden via pg_temp in the OSD Map.
```

根据CRUSH算法生成的负责特定版本及特定归置组的OSD有序列表。

通常情况下与*Acting Set*相同，除非*Acting Set*已被OSD Map中的`pg_temp`显式覆盖。


# 参考
 * [Ceph中一些PG相关的状态说明和基本概念说明](https://www.ichenfu.com/2018/07/06/ceph-pg-states-concepts/)
 * [PLACEMENT GROUP CONCEPTS](https://docs.ceph.com/en/latest/rados/operations/pg-concepts/)
