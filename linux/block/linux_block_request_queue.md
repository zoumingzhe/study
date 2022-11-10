linux block device request_queue
================================


# queue_limits
```C
struct queue_limits {
	enum blk_bounce		bounce;
	unsigned long		seg_boundary_mask;
	unsigned long		virt_boundary_mask;

	unsigned int		max_hw_sectors;
	unsigned int		max_dev_sectors;
	unsigned int		chunk_sectors;
	unsigned int		max_sectors;
	unsigned int		max_segment_size;
	unsigned int		physical_block_size;
	unsigned int		logical_block_size;
	unsigned int		alignment_offset;
	unsigned int		io_min;
	unsigned int		io_opt;
	unsigned int		max_discard_sectors;
	unsigned int		max_hw_discard_sectors;
	unsigned int		max_secure_erase_sectors;
	unsigned int		max_write_zeroes_sectors;
	unsigned int		max_zone_append_sectors;
	unsigned int		discard_granularity;
	unsigned int		discard_alignment;
	unsigned int		zone_write_granularity;

	unsigned short		max_segments;
	unsigned short		max_integrity_segments;
	unsigned short		max_discard_segments;

	unsigned char		misaligned;
	unsigned char		discard_misaligned;
	unsigned char		raid_partial_stripes_expensive;
	enum blk_zoned_model	zoned;
};
```

## queue_limits default values
```C
/**
 * blk_set_default_limits - reset limits to default values
 * @lim:  the queue_limits structure to reset
 *
 * Description:
 *   Returns a queue_limit struct to its default state.
 */
void blk_set_default_limits(struct queue_limits *lim)
{
	lim->max_segments = BLK_MAX_SEGMENTS;
	lim->max_discard_segments = 1;
	lim->max_integrity_segments = 0;
	lim->seg_boundary_mask = BLK_SEG_BOUNDARY_MASK;
	lim->virt_boundary_mask = 0;
	lim->max_segment_size = BLK_MAX_SEGMENT_SIZE;
	lim->max_sectors = lim->max_hw_sectors = BLK_SAFE_MAX_SECTORS;
	lim->max_dev_sectors = 0;
	lim->chunk_sectors = 0;
	lim->max_write_zeroes_sectors = 0;
	lim->max_zone_append_sectors = 0;
	lim->max_discard_sectors = 0;
	lim->max_hw_discard_sectors = 0;
	lim->max_secure_erase_sectors = 0;
	lim->discard_granularity = 0;
	lim->discard_alignment = 0;
	lim->discard_misaligned = 0;
	lim->logical_block_size = lim->physical_block_size = lim->io_min = 512;
	lim->bounce = BLK_BOUNCE_NONE;
	lim->alignment_offset = 0;
	lim->io_opt = 0;
	lim->misaligned = 0;
	lim->zoned = BLK_ZONED_NONE;
	lim->zone_write_granularity = 0;
}
EXPORT_SYMBOL(blk_set_default_limits);
```

## queue_limits logical_block_size
logical_block_size 描述硬件进行地址寻址的最小单元，其默认值为 512 bytes，对应 `/sys/block/<dev>/queue/logical_block_size`

对于 HDD 设备来说，设备能够寻址的最小存储单元是扇区 (sector)，每个扇区的大小是 512 bytes，因而 HDD 设备的 logical_block_size 属性就是 512 bytes

为了使用 block buffer 特性，文件系统的 block size 必须为块设备的 logical_block_size 的整数倍

同时向 device controller 下发的 IO 也必须是按照 logical_block_size 对齐

## queue_limits physical_block_size
physical_block_size 描述硬件执行写操作的最小单元，其默认值为 512 bytes，对应 `/sys/block/<dev>/queue/physical_block_size`

physical_block_size 必须是 logical_block_size 的整数倍

## queue_limits io_min
描述执行 IO 操作的最小单位，默认值为 512 bytes，对应 `/sys/block/<dev>/queue/minimum_io_size

Storage devices may report a granularity or preferred minimum I/O size which is the smallest request the device can perform without incurring a performance penalty.  For disk drives this is often the physical block size.  For RAID arrays it is often the stripe chunk size.  A properly aligned multiple of minimum_io_size is the preferred request size for workloads where a high number of I/O operations is desired.

## queue_limits io_opt
描述执行 IO 操作的最佳大小，默认值为 0，对应 `/sys/block/<dev>/queue/optimal_io_size`

Storage devices may report an optimal I/O size, which is the device's preferred unit for sustained I/O.  This is rarely reported for disk drives.  For RAID arrays it is usually the stripe width or the internal track size.  A properly aligned multiple of optimal_io_size is the preferred request size for workloads where sustained throughput is desired.



# 参考
 * [BLOCK 层这么多参数都是什么意思？！](https://developer.aliyun.com/article/784610)
