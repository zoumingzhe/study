# rbd crash

## symbolic

Loads symbolic and debugging data:

```text
crash> mod -s rbd
```

## view structure

### rbd_device

所有`rbd_device`都在`rbd_dev_list`链中，使用`list`命令查看链表：

```shell
list -H rbd_dev_list
```

或者，查看链表中所有`rbd_device`的`name`成员：

```shell
list -s rbd_device.name -l rbd_device.node -H rbd_dev_list
```

### rbd_client

所有`rbd_client`都在`rbd_client_list`链中，使用`list`命令查看链表：

```shell
list -H rbd_client_list
```

或者，查看链表中所有`rbd_client`的`client`（ceph_client）成员：

```shell
list -s rbd_client.client -l rbd_client.node -H rbd_client_list
```

#### 通过 rbd_device 查看 rbd_client

查看指定`rbd_device`的`rbd_client`成员：

```shell
rbd_device.rbd_client -l rbd_device.node ffffa02f072bd598
```

或者，查看`rbd_dev_list`链中所有`rbd_device`的`rbd_client`成员：

```shell
list -s rbd_device.rbd_client -l rbd_device.node -H rbd_dev_list
```

### ceph_client

查看指定`rbd_client`（或者指定`rbd_device`的`rbd_client`）的`ceph_client`成员：

```text
crash> rbd_device.rbd_client -l rbd_device.node ffffa02f072bd598
  rbd_client = 0xffff802f3a01bf80
crash> struct rbd_client.client 0xffff802f3a01bf80
  client = 0xffff802e0cfd6000
```

或者，查看`rbd_client_list`链中所有`rbd_client`的`client`（ceph_client）成员：

```shell
list -s rbd_client.client -l rbd_client.node -H rbd_client_list
```

或者，查看`rbd_dev_list`链中所有`rbd_device`的`client`（ceph_client）成员：

```shell
list -s rbd_device.rbd_client -l rbd_device.node -H rbd_dev_list | grep rbd_client | awk '{print "struct rbd_client.client " $3}'
```

### ceph_osd_client

查看指定`ceph_client`（或者指定`rbd_client`的`ceph_client`）的`osdc`（ceph_osd_client）成员：

```text
crash> rbd_device.rbd_client -l rbd_device.node ffffa02f072bd598
  rbd_client = 0xffff802f3a01bf80
crash> struct rbd_client.client 0xffff802f3a01bf80
  client = 0xffff802e0cfd6000
crash> struct ceph_client.osdc.client 0xffff802e0cfd6000
  osdc.client = 0xffff802e0cfd6000,
```

或者，查看`rbd_client_list`链中所有`ceph_client`的`osdc`（ceph_osd_client）成员：

```shell
list -s rbd_client.client -l rbd_client.node -H rbd_client_list | grep client | awk '{print "struct ceph_client.osdc.client " $3}'
```

#### 查看 num_requests

```text
crash> rbd_device.rbd_client -l rbd_device.node ffffa02f072bd598
  rbd_client = 0xffff802f3a01bf80
crash> struct rbd_client.client 0xffff802f3a01bf80
  client = 0xffff802e0cfd6000
crash> struct ceph_client.osdc.client 0xffff802e0cfd6000
  osdc.client = 0xffff802e0cfd6000,
crash> struct ceph_client.osdc.num_requests 0xffff802e0cfd6000
  osdc.num_requests = {
    counter = 0
  },
```

或者

```shell
list -s rbd_client.client -l rbd_client.node -H rbd_client_list | grep client | awk '{print "struct ceph_client.osdc.num_requests " $3}'
```
