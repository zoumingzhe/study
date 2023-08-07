# bcache crash

## symbolic

Loads symbolic and debugging data:

```text
crash> mod -s bcache
crash> sym -m bcache
```

## view structure

### cache_set

所有 cache_set 都在 bch_cache_sets 链中：

```text
crash> struct list_head bch_cache_sets
struct list_head {
  next = 0xffffa02fc4e60050,
  prev = 0xffff802f041e0050
}
```

使用 list 命令查看链表：

```text
crash> list bch_cache_sets
ffffffffc0248830
ffff985f5bae0050
ffff985f63760050
ffff985f58880050
ffff985fbd600050
```

或者，通过 "next" 逐个查看所有 cache_set 地址：

```text
crash> struct list_head bch_cache_sets
struct list_head {
  next = 0xffffa02fc4e60050,
  prev = 0xffff802f041e0050
}
crash> struct list_head 0xffffa02fc4e60050
struct list_head {
  next = 0xffff802f055c0050,
  prev = 0xffff0000089d0700
}
crash> struct list_head 0xffff802f055c0050
struct list_head {
  next = 0xffff802f041e0050,
  prev = 0xffffa02fc4e60050
}
crash> struct list_head 0xffff802f041e0050
struct list_head {
  next = 0xffff0000089d0700,
  prev = 0xffff802f055c0050
}
crash> struct list_head 0xffff0000089d0700
struct list_head {
  next = 0xffffa02fc4e60050,
  prev = 0xffff802f041e0050
}
```

通过 "cache_set.list" 地址查看 cache_set 信息：

```text
crash> cache_set -l "cache_set.list" 0xffffa02fc4e60050
```

或者直接查看 "cache_set.journal" 信息：

```text
crash> cache_set.journal -l "cache_set.list" 0xffffa02fc4e60050
```
