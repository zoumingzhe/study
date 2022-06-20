[bcache RTFSC](./) btree
========================

# bkey（btree keys）
bkey由高64位、低64位，以及一个64位数组组成。
```c
/* Btree keys - all units are in sectors */

struct bkey {
        __u64   high;
        __u64   low;
        __u64   ptr[];
};

/*
 * The high bit being set is a relic from when we used it to do binary
 * searches - it told you where a key started. It's not used anymore,
 * and can probably be safely dropped.
 */
#define KEY(inode, offset, size)                                        \
((struct bkey) {                                                        \
        .high = (1ULL << 63) | ((__u64) (size) << 20) | (inode),        \
        .low = (offset)                                                 \
})

#define ZERO_KEY                        KEY(0, 0, 0)

#define MAX_KEY_INODE                   (~(~0 << 20))
#define MAX_KEY_OFFSET                  (~0ULL >> 1)
#define MAX_KEY                         KEY(MAX_KEY_INODE, MAX_KEY_OFFSET, 0)
```

数据结构示意如下图：
![bcache_bkey](../bcache_btree/bcache_bkey/bcache_bkey.drawio.png)

## bkey pad
### BKEY_PAD
```c
/* Enough for a key with 6 pointers */
#define BKEY_PAD                8
```

### BKEY_PADDED
```c
#define BKEY_PADDED(key)                                        \
        union { struct bkey key; __u64 key ## _pad[BKEY_PAD]; }
```

## bkey key
### KEY_FIELD
bkey的高64位是由一系列位域组成。
```c
#define KEY_FIELD(name, field, offset, size)                            \
        BITMASK(name, struct bkey, field, offset, size)

#define KEY_SIZE_BITS           16
#define KEY_MAX_U64S            8

KEY_FIELD(KEY_PTRS,     high, 60, 3)
KEY_FIELD(__PAD0,       high, 58, 2)
KEY_FIELD(KEY_CSUM,     high, 56, 2)
KEY_FIELD(__PAD1,       high, 55, 1)
KEY_FIELD(KEY_DIRTY,    high, 36, 1)

KEY_FIELD(KEY_SIZE,     high, 20, KEY_SIZE_BITS)
KEY_FIELD(KEY_INODE,    high, 0,  20)
```

### KEY_INODE
`KEY_INODE`是bkey数据对应backing的设备ID，共20位。这意味着理论上最多可以存在超过100w（1048576）个inode，但由于uuid槽位数的限制，实际上inode并不会有这么多。

### KEY_SIZE
`KEY_SIZE`是bkey数据对应的长度，共16位，单位sector。这意味着一个bkey最多可以有65536个sector，1个sector为512字节，则一个bkey最多表达32M数据。

### KEY_OFFSET
bkey的低64位是`KEY_OFFSET`，单位sector。它是bkey数据对应backing的截止位置，而不是对应的起始位置，这一点需要特别注意。
```c
/* Next time I change the on disk format, KEY_OFFSET() won't be 64 bits */

static inline __u64 KEY_OFFSET(const struct bkey *k)
{
        return k->low;
}
```

### KEY_START
`KEY_START`是bkey数据对应backing的起始位置，单位sector。它需要由`KEY_OFFSET`减去`KEY_SIZE`计算得到。
```c
static inline void SET_KEY_OFFSET(struct bkey *k, __u64 v)
{
        k->low = v;
}

#define KEY_START(k)                    (KEY_OFFSET(k) - KEY_SIZE(k))
```

### KEY_DIRTY
若为writeback写，`bch_data_insert_start`会在插入的bkey前将dirty置位。
```c
static void bch_data_insert_start(struct closure *cl)
{
        ...

        do {
                ...

                if (op->writeback) {
                        SET_KEY_DIRTY(k, true);

                        for (i = 0; i < KEY_PTRS(k); i++)
                                SET_GC_MARK(PTR_BUCKET(op->c, k, i),
                                            GC_MARK_DIRTY);
                }

                ...
        } while (n != bio);

        ...
}
```

当`write_dirty_finish`时，若dirty被置位则将其清除并重新插入bkey。
```c
static void write_dirty_finish(struct closure *cl)
{
        ...

        /* This is kind of a dumb way of signalling errors. */
        if (KEY_DIRTY(&w->key)) {
                ...
                struct keylist keys;

                bch_keylist_init(&keys);

                bkey_copy(keys.top, &w->key);
                SET_KEY_DIRTY(keys.top, false);
                bch_keylist_push(&keys);

                ...

                ret = bch_btree_insert(dc->disk.c, &keys, NULL, &w->key);

                if (ret)
                        trace_bcache_writeback_collision(&w->key);

                atomic_long_inc(ret
                                ? &dc->disk.c->writeback_keys_failed
                                : &dc->disk.c->writeback_keys_done);
        }

        ...
}
```

需要注意的是，`dirty_endio`中将dirty清除只是用来标识`write_dirty`的io_error。因为，在`write_dirty_finish`中通过dirty标志位来判断是否出现io_error。同时，此处的bkey空间是为`write_dirty`而开辟的新空间，清除的并不是btree中实际的bkey上的dirty，所以不会有数据损坏的风险。
```c
static void dirty_endio(struct bio *bio)
{
        struct keybuf_key *w = bio->bi_private;
        struct dirty_io *io = w->private;

        if (bio->bi_status) {
                SET_KEY_DIRTY(&w->key, false);
                bch_count_backing_io_errors(io->dc, bio);
        }

        closure_put(&io->cl);
}
```

## bkey pointer
### PTR_FIELD
```c
#define PTR_FIELD(name, offset, size)                                   \
static inline __u64 name(const struct bkey *k, unsigned int i)          \
{ return (k->ptr[i] >> offset) & ~(~0ULL << size); }                    \
                                                                        \
static inline void SET_##name(struct bkey *k, unsigned int i, __u64 v)  \
{                                                                       \
        k->ptr[i] &= ~(~(~0ULL << size) << offset);                     \
        k->ptr[i] |= (v & ~(~0ULL << size)) << offset;                  \
}

#define PTR_DEV_BITS                    12

PTR_FIELD(PTR_DEV,                      51, PTR_DEV_BITS)
PTR_FIELD(PTR_OFFSET,                   8,  43)
PTR_FIELD(PTR_GEN,                      0,  8)

#define PTR_CHECK_DEV                   ((1 << PTR_DEV_BITS) - 1)

#define MAKE_PTR(gen, offset, dev)                                      \
        ((((__u64) dev) << 51) | ((__u64) offset) << 8 | gen)
```

### PTR_DEV

### PTR_GEN

### PTR_OFFSET

### PTR_CACHE
```c
static inline struct cache *PTR_CACHE(struct cache_set *c,
                                      const struct bkey *k,
                                      unsigned int ptr)
{
        return c->cache[PTR_DEV(k, ptr)];
}
```

### PTR_BUCKET
```c
static inline size_t sector_to_bucket(struct cache_set *c, sector_t s)
{
        return s >> c->bucket_bits;
}

static inline size_t PTR_BUCKET_NR(struct cache_set *c,
                                   const struct bkey *k,
                                   unsigned int ptr)
{
        return sector_to_bucket(c, PTR_OFFSET(k, ptr));
}

static inline struct bucket *PTR_BUCKET(struct cache_set *c,
                                        const struct bkey *k,
                                        unsigned int ptr)
{
        return PTR_CACHE(c, k, ptr)->buckets + PTR_BUCKET_NR(c, k, ptr);
}
```

## bkey utility

### bkey_bytes
```c
static inline unsigned long bkey_u64s(const struct bkey *k)
{
        return (sizeof(struct bkey) / sizeof(__u64)) + KEY_PTRS(k);
}

static inline unsigned long bkey_bytes(const struct bkey *k)
{
        return bkey_u64s(k) * sizeof(__u64);
}
```

### bkey_init
```c
static inline void bkey_init(struct bkey *k)
{
        *k = ZERO_KEY;
}
```

### bkey_cmp
```c
static __always_inline int64_t bkey_cmp(const struct bkey *l,
                                        const struct bkey *r)
{
        return unlikely(KEY_INODE(l) != KEY_INODE(r))
                ? (int64_t) KEY_INODE(l) - (int64_t) KEY_INODE(r)
                : (int64_t) KEY_OFFSET(l) - (int64_t) KEY_OFFSET(r);
}
```

### bkey_copy
```c
#define bkey_copy(_dest, _src)	memcpy(_dest, _src, bkey_bytes(_src))

static inline void bkey_copy_key(struct bkey *dest, const struct bkey *src)
{
        SET_KEY_INODE(dest, KEY_INODE(src));
        SET_KEY_OFFSET(dest, KEY_OFFSET(src));
}
```

### bkey_next
```c
static inline struct bkey *bkey_next(const struct bkey *k)
{
        __u64 *d = (void *) k;
        return (struct bkey *) (d + bkey_u64s(k));
}
```

### bkey_idx
```c
static inline struct bkey *bkey_idx(const struct bkey *k, unsigned nr_keys)
{
        __u64 *d = (void *) k;
        return (struct bkey *) (d + nr_keys);
}
```

## keylist
```c
struct keylist {
        union {
                struct bkey             *keys;
                uint64_t                *keys_p;
        };
        union {
                struct bkey             *top;
                uint64_t                *top_p;
        };

        /* Enough room for btree_split's keys without realloc */
#define KEYLIST_INLINE          16
        uint64_t                inline_keys[KEYLIST_INLINE];
};
```

### bch_keylist_init
```c
static inline void bch_keylist_init(struct keylist *l)
{
        l->top_p = l->keys_p = l->inline_keys;
}

static inline void bch_keylist_init_single(struct keylist *l, struct bkey *k)
{
        l->keys = k;
        l->top = bkey_next(k);
}
```

# 参考
 * []()
