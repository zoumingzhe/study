[bcache RTFSC](./) journal
==========================


# journal bucket
## SB_JOURNAL_BUCKETS
journal最多可以有256个bucket，这是由`SB_JOURNAL_BUCKETS`定义的。
```C
#define SB_JOURNAL_BUCKETS        256U
```

## njournal_buckets
`njournal_buckets`是journal的实际bucket个数，应为`ca->sb.nbuckets >> 7`，若不在`[2, SB_JOURNAL_BUCKETS]`范围内则就近取值。
同时，journal的bucket空间应为紧跟`ca->sb.first_bucket`之后的一段连续区域。
```C
pr_notice("invalidating existing data");

for_each_cache(ca, c, i) {
        unsigned int j;

        ca->sb.keys = clamp_t(int, ca->sb.nbuckets >> 7,
                              2, SB_JOURNAL_BUCKETS);

        for (j = 0; j < ca->sb.keys; j++)
                ca->sb.d[j] = ca->sb.first_bucket + j;
}
```

# journal read
## bch_journal_read()
`bch_journal_read()`以bucket为粒度读取日志。首先，按黄金分割点哈希读取日志bucket，如果读到有效的日志项则开始搜索所有日志项；如果没有读到有效的日志项则需要检查所有未读的日志bucket。
```C
int bch_journal_read(struct cache_set *c, struct list_head *list)
{
#define read_bucket(b)                                                  \
        ({                                                              \
                ret = journal_read_bucket(ca, list, b);                 \
                __set_bit(b, bitmap);                                   \
                if (ret < 0)                                            \
                        return ret;                                     \
                ret;                                                    \
        })

        ......

        for_each_cache(ca, c, iter) {
                ......

                /*
                 * Read journal buckets ordered by golden ratio hash to quickly
                 * find a sequence of buckets with valid journal entries
                 */
                for (i = 0; i < ca->sb.njournal_buckets; i++) {
                        /*
                         * We must try the index l with ZERO first for
                         * correctness due to the scenario that the journal
                         * bucket is circular buffer which might have wrapped
                         */
                        l = (i * 2654435769U) % ca->sb.njournal_buckets;

                        if (test_bit(l, bitmap))
                                break;

                        if (read_bucket(l))
                                goto bsearch;
                }

                /*
                 * If that fails, check all the buckets we haven't checked
                 * already
                 */
                pr_debug("falling back to linear search");

                for (l = find_first_zero_bit(bitmap, ca->sb.njournal_buckets);
                     l < ca->sb.njournal_buckets;
                     l = find_next_zero_bit(bitmap, ca->sb.njournal_buckets,
                                            l + 1))
                        if (read_bucket(l))
                                goto bsearch;

                /* no journal entries on this device? */
                if (l == ca->sb.njournal_buckets)
                        continue;
bsearch:
                BUG_ON(list_empty(list));

                /* Binary search */

                ......
        }

        ......

        return 0;
#undef read_bucket
}
```


## journal_read_bucket()
`bch_journal_read()`读取bucket中的日志项。
```C
static inline sector_t bucket_to_sector(struct cache_set *c, size_t b)
{
        return ((sector_t) b) << c->bucket_bits;
}

static int journal_read_bucket(struct cache *ca, struct list_head *list,
                               unsigned bucket_index)
{
        ......
        int ret = 0;
        sector_t bucket = bucket_to_sector(ca->set, ca->sb.d[bucket_index]);
        ......

        while (offset < ca->sb.bucket_size) {
reread:         left = ca->sb.bucket_size - offset;
                len = min_t(unsigned int, left, PAGE_SECTORS << JSET_BITS);

                bio_reset(bio);
                bio->bi_iter.bi_sector  = bucket + offset;
                bio_set_dev(bio, ca->bdev);
                bio->bi_iter.bi_size    = len << 9;

                bio->bi_end_io  = journal_read_endio;
                bio->bi_private = &cl;
                bio_set_op_attrs(bio, REQ_OP_READ, 0);
                bch_bio_map(bio, data);

                closure_bio_submit(ca->set, bio, &cl);
                closure_sync(&cl);

                ......

                j = data;
                while (len) {
                        struct list_head *where;
                        size_t blocks, bytes = set_bytes(j);

                        if (j->magic != jset_magic(&ca->sb)) {
                                pr_debug("%u: bad magic", bucket_index);
                                return ret;
                        }

                        if (bytes > left << 9 ||
                            bytes > PAGE_SIZE << JSET_BITS) {
                                pr_info("%u: too big, %zu bytes, offset %u",
                                        bucket_index, bytes, offset);
                                return ret;
                        }

                        if (bytes > len << 9)
                                goto reread;

                        if (j->csum != csum_set(j)) {
                                pr_info("%u: bad csum, %zu bytes, offset %u",
                                        bucket_index, bytes, offset);
                                return ret;
                        }

                        blocks = set_blocks(j, block_bytes(ca->set));
                        ......

                        ......
next_set:
                        offset  += blocks * ca->sb.block_size;
                        len     -= blocks * ca->sb.block_size;
                        j = ((void *) j) + blocks * block_bytes(ca);
                }
        }
}
```


# 参考
 * []()
