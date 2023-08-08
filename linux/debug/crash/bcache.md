# bcache crash

## symbolic

Loads symbolic and debugging data:

```text
crash> mod -s bcache
crash> sym -m bcache
```

## view structure

### cache_set

所有`cache_set`都在`bch_cache_sets`链中，使用`list`命令查看链表：

```text
crash> list -H bch_cache_sets
ffff802f70dc0050
ffffa02f74da0050
ffffa03fdd540050
ffffa03fde4c0050
```

或者，查看链表中所有结构体对象的成员：

```shell
list -s cache_set.nr_uuids -l cache_set.list -H bch_cache_sets
```

通过`cache_set.list`地址查看`cache_set`的完整结构体：

```shell
cache_set -l cache_set.list 0xffffa02fc4e60050
```

或者，直接直接查看结构体下的`cache_set.journal`成员：

```shell
cache_set.journal -l cache_set.list 0xffffa02fc4e60050
```

### cached_dev

所有`attach`状态`cached_dev`都在`cache_set`下的`cached_devs`链中：

```shell
list -s cache_set.cached_devs -l cache_set.list -H bch_cache_sets
```

或者，所有`detach`状态`cached_dev`都在`uncached_devices`链中：

```shell
list -H uncached_devices
```

#### 查看 cache_set.cached_devs 链表

```shell
list -s cached_dev.has_dirty -l cached_dev.list ffff803f25310000
```

#### 查找 cached_dev 下的所有 dirty_io

```shell
search -k ffff803ef4d60000 | awk -F ':' '{print "dirty_io.cl.fn -l dirty_io.dc " $1}'
```
