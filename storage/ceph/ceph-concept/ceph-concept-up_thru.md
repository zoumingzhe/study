up_thru
=======
before a primary can successfully complete the peering process, it must inform a monitor that is alive through the current OSD map epoch by having the monitor set its up_thru in the osd map. This helps peering ignore previous acting sets for which peering never completed after certain sequences of failures, such as the second interval below:
 - acting set = [A,B]
 - acting set = [A]
 - acting set = [] very shortly after (e.g., simultaneous failure, but staggered detection)
 - acting set = [B] (B restarts, A does not)

在`primary`成功完成`peering`之前，它必须通过让monitor在当前`epoch`的`OSD Map`中设置其`up_thru`来通知monitor其活着。这有助于`peering`忽略那些在一系列故障后`peering`从未完成的`Acting Set`。例如：
 - acting set = [A,B]
 - acting set = [A]
 - acting set = [] （时间非常短，例如：同时发生故障，但是发现错开）
 - acting set = [B] （B重启，A未重启）

# 参考
 * [PEERING](https://docs.ceph.com/en/latest/dev/peering/)
