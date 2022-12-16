missing set
===========
Each OSD notes update log entries and if they imply updates to the contents of an object, adds that object to a list of needed updates. This list is called the missing set for that <OSD,PG>.

每个`OSD`都会记录更新日志条目，如果这些条目表明更新某一对象的内容则将该对象添加到一个需要更新的列表中。此列表被称为<OSD，PG>的`missing set`。

# 参考
 * [PEERING](https://docs.ceph.com/en/latest/dev/peering/)
