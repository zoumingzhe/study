Up Set （当选集合）
==================
The ordered list of OSDs responsible for a particular placement group for a particular epoch according to CRUSH. Normally this is the same as the Acting Set, except when the Acting Set has been explicitly overridden via pg_temp in the OSD Map.

根据`CRUSH`算法生成的负责特定版本及特定`PG`的`OSD`有序列表。

通常情况下与`Acting Set`相同，除非`Acting Set`已被`OSD Map`中的`pg_temp`显式覆盖。

# 参考
 * [PLACEMENT GROUP CONCEPTS](https://docs.ceph.com/en/latest/rados/operations/pg-concepts/)
 * [PEERING](https://docs.ceph.com/en/latest/dev/peering/)
