[bcache RTFSC](./) uuid
=======================

bcache的uuid为16字节二进制（36字节字符）组成的唯一标识符，由make-bcache使用libuuid库生成（亦可通过参数指定），有cset.uuid和dev.uuid两种，其目的是对不同的cache和backing设备进行唯一区分，并建立相应的映射关系。

```shell
[root@zmz-test ~]# lsblk -s /dev/bcache0
NAME     MAJ:MIN RM  SIZE RO TYPE MOUNTPOINT
bcache0  252:0    0 1024M  0 disk 
├─vdb    253:80   0    1G  0 disk 
└─vdc    253:32   0    1G  0 disk
[root@zmz-test ~]# bcache-super-show /dev/vdb
sb.magic                ok
sb.first_sector         8 [match]
sb.csum                 A5D02988AEA426BE [match]
sb.version              4 [backing device]

dev.label               (empty)
dev.uuid                066afcc4-422e-4d7d-b567-d387be8fbc30
dev.sectors_per_block   1
dev.sectors_per_bucket  1024
dev.data.first_sector   24
dev.data.cache_mode     0 [writethrough]
dev.data.cache_state    1 [clean]

cset.uuid               506b1176-efc9-438a-a58e-b8f780f34e09
[root@zmz-test ~]# bcache-super-show /dev/vdc
sb.magic                ok
sb.first_sector         8 [match]
sb.csum                 FBA81FD38EF3C1B5 [match]
sb.version              3 [cache device]

dev.label               (empty)
dev.uuid                f199d0a4-77a2-4981-86ce-c70d0bf984a3
dev.sectors_per_block   1
dev.sectors_per_bucket  1024
dev.cache.first_sector  1024
dev.cache.cache_sectors 2096128
dev.cache.total_sectors 2097152
dev.cache.ordered       yes
dev.cache.discard       no
dev.cache.pos           0
dev.cache.replacement   0 [lru]

cset.uuid               506b1176-efc9-438a-a58e-b8f780f34e09
```

# cset.uuid（cache_set uuid）
```c
```

# dev.uuid（device uuid）
## bdev.uuid（backing device uuid）
`struct uuid_entry`是bdev.uuid落盘数据结构，为了对齐与预留空间，每个uuid占用128字节。
```c 
struct uuid_entry {
        union {
                struct {
                        __u8    uuid[16];
                        __u8    label[32];
                        __u32   first_reg; /* time overflow in y2106 */
                        __u32   last_reg;
                        __u32   invalidated;

                        __u32   flags;
                        /* Size of flash only volumes */
                        __u64   sectors;
                };

                __u8            pad[128];
        };
};
```

`struct uuid_entry_v0`是旧版本的元数据结构，可以向前兼容。
```c
/* UUIDS - per backing device/flash only volume metadata */

struct uuid_entry_v0 {
        __u8            uuid[16];
        __u8            label[32];
        __u32           first_reg;
        __u32           last_reg;
        __u32           invalidated;
        __u32           pad;
};
```

每个cache_set所能容纳的uuid槽位数量由bucket大小决定，即所有uuid槽位占用一个bucket空间。
```c
struct cache_set *bch_cache_set_alloc(struct cache_sb *sb)
{
        ......
        c->nr_uuids = meta_bucket_bytes(sb) / sizeof(struct uuid_entry);
        ......

        ......
        c->uuids = alloc_meta_bucket_pages(GFP_KERNEL, sb);
        if (!c->uuids)
                goto err;
        ......

}
```

## uuid_read
```c
static char *uuid_read(struct cache_set *c, struct jset *j, struct closure *cl)
{
        struct bkey *k = &j->uuid_bucket;

        if (__bch_btree_ptr_invalid(c, k))
                return "bad uuid pointer";

        bkey_copy(&c->uuid_bucket, k);
        uuid_io(c, REQ_OP_READ, 0, k, cl);

        if (j->version < BCACHE_JSET_VERSION_UUIDv1) {
                struct uuid_entry_v0    *u0 = (void *) c->uuids;
                struct uuid_entry       *u1 = (void *) c->uuids;
                int i;

                closure_sync(cl);

                /*
                 * Since the new uuid entry is bigger than the old, we have to
                 * convert starting at the highest memory address and work down
                 * in order to do it in place
                 */

                for (i = c->nr_uuids - 1;
                     i >= 0;
                     --i) {
                        memcpy(u1[i].uuid,      u0[i].uuid, 16);
                        memcpy(u1[i].label,     u0[i].label, 32);

                        u1[i].first_reg         = u0[i].first_reg;
                        u1[i].last_reg          = u0[i].last_reg;
                        u1[i].invalidated       = u0[i].invalidated;

                        u1[i].flags     = 0;
                        u1[i].sectors   = 0;
                }
        }

        return NULL;
}

```

## uuid_write
```c
static int __uuid_write(struct cache_set *c)
{
        BKEY_PADDED(key) k;
        struct closure cl;
        struct cache *ca = c->cache;
        unsigned int size;

        closure_init_stack(&cl);
        lockdep_assert_held(&bch_register_lock);

        if (bch_bucket_alloc_set(c, RESERVE_BTREE, &k.key, true))
                return 1;

        size =  meta_bucket_pages(&ca->sb) * PAGE_SECTORS;
        SET_KEY_SIZE(&k.key, size);
        uuid_io(c, REQ_OP_WRITE, 0, &k.key, &cl);
        closure_sync(&cl);

        /* Only one bucket used for uuid write */
        atomic_long_add(ca->sb.bucket_size, &ca->meta_sectors_written);

        bkey_copy(&c->uuid_bucket, &k.key);
        bkey_put(c, &k.key);
        return 0;
}

int bch_uuid_write(struct cache_set *c)
{
        int ret = __uuid_write(c);

        if (!ret)
                bch_journal_meta(c, NULL);

        return ret;
}
```

## uuid_find
`uuid_find()`用于从uuid槽位中查找第一个匹配位置。
```c
static struct uuid_entry *uuid_find(struct cache_set *c, const char *uuid)
{
        struct uuid_entry *u;

        for (u = c->uuids;
             u < c->uuids + c->nr_uuids; u++)
                if (!memcmp(u->uuid, uuid, 16))
                        return u;

        return NULL;
}
```

`uuid_find_empty()`用于从uuid槽位中查找一个新的槽位（zero_uuid）。
```c
static struct uuid_entry *uuid_find_empty(struct cache_set *c)
{
        static const char zero_uuid[16] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
        return uuid_find(c, zero_uuid);
}
```

attach设备时，首先根据bdev.uuid从uuid槽位中查找。若找到则根据`BDEV_STATE`判断是否需要重新分配；若没有找到则从uuid槽位中分配一个新的槽位（zero_uuid），但是backing设备脏（状态为`BDEV_STATE_DIRTY`）时不能为其分配。
```c
int bch_cached_dev_attach(struct cached_dev *dc, struct cache_set *c,
                          uint8_t *set_uuid)
{
        ......
        u = uuid_find(c, dc->sb.uuid);

        if (u &&
            (BDEV_STATE(&dc->sb) == BDEV_STATE_STALE ||
             BDEV_STATE(&dc->sb) == BDEV_STATE_NONE)) {
                memcpy(u->uuid, invalid_uuid, 16);
                u->invalidated = cpu_to_le32((u32)ktime_get_real_seconds());
                u = NULL;
        }

        if (!u) {
                if (BDEV_STATE(&dc->sb) == BDEV_STATE_DIRTY) {
                        pr_err("Couldn't find uuid for %pg in set\n", dc->bdev);
                        return -ENOENT;
                }

                u = uuid_find_empty(c);
                if (!u) {
                        pr_err("Not caching %pg, no room for UUID\n", dc->bdev);
                        return -EINVAL;
                }
        }
        ......
}
```

## invalid_uuid
invalid_uuid作为一个特殊的uuid，用来标记那些曾经被使用但已经释放的uuid槽位。
```c
static const char invalid_uuid[] = {
        0xa0, 0x3e, 0xf8, 0xed, 0x3e, 0xe1, 0xb8, 0x78,
        0xc8, 0x50, 0xfc, 0x5e, 0xcb, 0x16, 0xcd, 0x99
};
```

detach设备时，需要将bdev.uuid从uuid槽位中清除（设为invalid_uuid）。在当前代码中这意味着该槽位被释放且将永远不再使用，除非对cache设备重新make-bcache。即uuid槽位虽然被释放但无法再次使用该槽位，社区对此做出了解释：“btree中bkey的inode域使用的是uuid槽位号，即使uuid槽位被释放，但是btree中仍然可能残留该inode的bkey。早期bcache不稳定，这也是一个规避措施。”所以，“feature not a bug”。反复的attach和detach最终会耗尽uuid槽位。
```c
static void bcache_device_detach(struct bcache_device *d)
{
        ......
        if (test_bit(BCACHE_DEV_DETACHING, &d->flags)) {
                struct uuid_entry *u = d->c->uuids + d->id;

                SET_UUID_FLASH_ONLY(u, 0);
                memcpy(u->uuid, invalid_uuid, 16);
                u->invalidated = cpu_to_le32((u32)ktime_get_real_seconds());
                bch_uuid_write(d->c);
        }
        ......
}
```

# 参考
 * []()
