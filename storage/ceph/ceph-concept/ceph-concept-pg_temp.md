PG temp
=======
a temporary placement group acting set used while backfilling the primary osd. Let say acting is [0,1,2] and we are active+clean. Something happens and acting is now [3,1,2]. osd 3 is empty and can’t serve reads although it is the primary. osd.3 will see that and request a PG temp of [1,2,3] to the monitors using a MOSDPGTemp message so that osd.1 temporarily becomes the primary. It will select osd.3 as a backfill peer and continue to serve reads and writes while osd.3 is backfilled. When backfilling is complete, PG temp is discarded and the acting set changes back to [3,1,2] and osd.3 becomes the primary.

回填`primary OSD`时使用的`PG`临时`Acting Set`。假设`Acting Set`是[0,1,2]，并且状态为active+clean。发生了一些事情，`Acting Set`现在是[3,1,2]。OSD.3虽然是`primary`，但是为空，所以无法提供读服务。OSD.3将看到这一情况并通过`MOSDPGTemp`消息向`monitor`请求一个[1,2,3]的`PG temp`，以便OSD.1临时成为`primary`。它将选择OSD.3作为回填peer，并在OSD.3被回填后继续提供读写服务。回填完成后，`PG temp`被丢弃，`Acting Set`变回[3,1,2]，OSD.3成为主要装置。

# 参考
 * [PEERING](https://docs.ceph.com/en/latest/dev/peering/)
