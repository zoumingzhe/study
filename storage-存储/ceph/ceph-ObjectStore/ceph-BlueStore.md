Ceph BlueStore
==============

# 映射
这非常粗略地将写入类型映射到遇到给定blob时所执行的操作。在实际情况会复杂一些，因为可能要考虑多个blob（例如，可能能够将*W*变成一个或将*P*变成另一个），但是它应该传达出一种粗略的策略构想。


BlueStore的每个Object都对应一个Onode，每个Onode包含一张extent-map，extent-map包含多个逻辑extent（lextent），用一个omap记录。

每个lextent负责管理Object内的一个逻辑段数据，并且通过blob的id关联一个Blob（bluestore_blob_t）。

Blob通过pextent（offset和length）定位物理盘上的实际位置。

一个onode里会存在多个lextent（逻辑的数据块），用一个omap记录。lextent通过blob的id对应blob（bluestore_blob_t），blob通过pextent（offset和length）定位物理盘上的实际位置。
## Onode
Bluestore里的任何一个Object都对应一个Onode（内存结构），定义如下：

./src/os/bluestore/BlueStore.h:bluestore_onode_t
```C++
/// onode: per-object metadata
struct bluestore_onode_t {
    uint64_t nid = 0;           ///< numeric id (locally unique)
    uint64_t size = 0;          ///< object size
    ···
};
```

./src/os/bluestore/BlueStore.h:Onode
```C++
struct Onode {
    ···
    Collection *c;              // 对应的Collection，对应PG
    ghobject_t oid;             // Object信息
    ···
    bluestore_onode_t onode;    // Object存到kv DB的元数据信息
    ···
    ExtentMap extent_map;       // 映射lextents到blobs
    ···
};
```

通过Onode里的ExtentMap来查询Object数据到底层的映射。

## ExtentMap
ExtentMap是Extent的有序集合，定义如下：

./src/os/bluestore/BlueStore.h:ExtentMap
```C++
struct ExtentMap {
    Onode *onode;                   // 指向Onode指针
    extent_map_t extent_map;        // Extents到Blobs的map
    blob_map_t spanning_blob_map;   // 跨越shards的blobs
 
    struct Shard {
        bluestore_onode_t::shard_info *shard_info = nullptr;
        unsigned extents = 0;  ///< count extents in this shard
        bool loaded = false;   ///< true if shard is loaded
        bool dirty = false;    ///< true if shard is dirty and needs reencoding
    };
    mempool::bluestore_cache_other::vector<Shard> shards;    ///< shards

    ···

};
```

ExtentMap提供了分片功能，防止在数据碎片化严重（ExtentMap很大）时，影响写RocksDB的性能。

 - ExtentMap随着写入数据可能会变化。
 - ExtentMap可能不连续，中间有空洞。
 - ExtentMap的连续小段会被合并成大段。
 - 覆盖写可能会导致ExtentMap分配新的Blob。

## Extent
Extent是实现Object的数据映射的关键数据结构，定义如下：

./src/os/bluestore/BlueStore.h:Extent
```C++
struct Extent : public ExtentBase {
    ···

    uint32_t logical_offset = 0;    // 对应Object的逻辑偏移
    uint32_t blob_offset = 0;       // 对应Blob上的偏移
    uint32_t length = 0;            // 数据段长度
    BlobRef  blob;                  // 指向对应Blob的指针

    ···
};
```

每个Extent都会映射到下一层的Blob上，Extent按block_size对齐，未写区域填零。

Extent中的length值，最小为block_size，最大为max_blob_size

## Blob
Blob是Bluestore里引入的处理块设备数据映射的中间层，定义如下：

./src/os/bluestore/bluestore_types.h:bluestore_blob_t
```C++
/// blob: a piece of data on disk
struct bluestore_blob_t {
private:
    PExtentVector extents;          // 对应磁盘上的一组数据段
    uint32_t logical_length = 0;    // blob的原始数据长度
    uint32_t compressed_length = 0; // 压缩的数据长度

public:
    ···
};
```

./src/os/bluestore/BlueStore.h:Blob
```C++
/// in-memory blob metadata and associated cached buffers (if any)
struct Blob {
    ···
    int16_t id = -1;
    ···
    SharedBlobRef shared_blob;      // 共享的blob状态

private:
    mutable bluestore_blob_t blob;  // blob的元数据
    ···
};
```

每个Blob会对应一组PExtentVector（bluestore_pextent_t的数组），指向从Disk中分配的物理空间。

 - Blob里可能对应一个pextent，也可能对应多个pextent。
 - Blob里的pextent个数最多为max_blob_size/min_alloc_size。
 - Blob里的多个pextent映射的Blob offset可能不连续，中间有空洞。

## PExtent
bluestore_pextent_t是管理物理磁盘上的数据段的，定义如下：

./src/os/bluestore/bluestore_types.h:bluestore_interval_t
```C++
template <typename OFFS_TYPE, typename LEN_TYPE>
struct bluestore_interval_t
{
    ...

    OFFS_TYPE offset = 0;   // 磁盘上的物理偏移
    LEN_TYPE length = 0;    // 数据段的长度

    ...
};
```

./src/os/bluestore/bluestore_types.h:bluestore_pextent_t
```C++
/// pextent: physical extent
struct bluestore_pextent_t : public bluestore_interval_t<uint64_t, uint32_t> 
{
    ...
};
```

bluestore_pextent_t的length值，最小为min_alloc_size，最大为max_blob_size。

# 粒度
## max_blob_size
Blob分配大小

## min_alloc_size
磁盘分配大小

# 写策略
## 小写策略
 * *U*：新blob的不压缩完整写。
    - 写入新blob
    - kv commit
 * *P*：现有blob的未使用区域（chunk(s)）的不压缩部分写。
    - 写入现有blob未使用chunk(s)
    - kv commit
 * *W*：WAL覆盖写：先提交覆盖写日志，然后异步覆盖写。必须按chunk_size = MAX(block_size, csum_block_size)对齐。
    - kv commit
    - WAL覆盖写现有blob（chunk对齐）
 * *N*：新blob的不压缩部分写。最初稀疏使用，随后会是*P*或*W*写。
    - 写入（稀疏的）新blob
    - kv commit
 * *R+W*：RMW（Read-Modify-Write）：部分chunk读，然后WAL覆盖写。
    - 读取（到chunk边界）
    - kv commit
    - WAL覆盖写现有blob（chunk对齐）
 * *C*：压缩数据，写入新的blob。
    - 压缩，写入新blob
    - kv commit

## 未来可能的模式
 * *F*：通过将少量数据写入零散的blob（收集需要写入的随机、不连续的数据位）来分割lextent空间
    - 写入零散的blob（min_alloc_size或更大，但仅使用其中的一个block）
    - kv commit
 * *X*：在单个block上（例如旧版bluestore）进行WAL读取/修改/写入。没有checksum。
    - kv commit
    - WAL读取/修改/写入

## 写流程
```
// 按照min_alloc_size大小切分，把写数据映射到不同的块上
          [offset, length]
          |==p1==|=======p2=======|=p3=|
|----------------|----------------|----------------|
| min_alloc_size | min_alloc_size | min_alloc_size |
|----------------|----------------|----------------|
small write: p1, p3
big   write: p2
 
 
BlueStore::_do_write()
|-- BlueStore::_do_write_data()
|   // 依据`min_alloc_size`把写切分为`small/big`写
|   | -- BlueStore::_do_write_small()
|   |    | -- BlueStore::ExtentMap::seek_lextent()
|   |    | -- BlueStore::Blob::can_reuse_blob()
|   |         reuse blob? or new blob?
|   |    | -- insert to struct WriteContext {};
|   | -- BlueStore::_do_write_big()
|   |    | -- BlueStore::ExtentMap::punch_hole()
|   |    | -- BlueStore::Blob::can_reuse_blob()
|   |         reuse blob? or new blob?
|   |    | -- insert to struct WriteContext {};
|-- BlueStore::_do_alloc_write()
|   | -- StupidAllocator::allocate()
|   | -- BlueStore::ExtentMap::set_lextent()
|   | -- BlueStore::_buffer_cache_write()
|-- BlueStore::_wctx_finish()
```

./src/os/bluestore/BlueStore.cc:BlueStore::_do_write()
```C++
```

./src/os/bluestore/BlueStore.cc:BlueStore::_do_write_data()
```C++
void BlueStore::_do_write_data(
  TransContext *txc,
  CollectionRef& c,
  OnodeRef o,
  uint64_t offset,
  uint64_t length,
  bufferlist& bl,
  WriteContext *wctx)
{
  uint64_t end = offset + length;
  bufferlist::iterator p = bl.begin();

  if (offset / min_alloc_size == (end - 1) / min_alloc_size &&
      (length != min_alloc_size)) {
    // we fall within the same block
    _do_write_small(txc, c, o, offset, length, p, wctx);
  } else {
    uint64_t head_offset, head_length;
    uint64_t middle_offset, middle_length;
    uint64_t tail_offset, tail_length;

    head_offset = offset;
    head_length = p2nphase(offset, min_alloc_size);

    tail_offset = p2align(end, min_alloc_size);
    tail_length = p2phase(end, min_alloc_size);

    middle_offset = head_offset + head_length;
    middle_length = length - head_length - tail_length;

    if (head_length) {
      _do_write_small(txc, c, o, head_offset, head_length, p, wctx);
    }

    _do_write_big(txc, c, o, middle_offset, middle_length, p, wctx);

    if (tail_length) {
      _do_write_small(txc, c, o, tail_offset, tail_length, p, wctx);
    }
  }
}
```

# 参考
 * [BLUESTORE INTERNALS](https://docs.ceph.com/en/latest/dev/bluestore/)
 * [BLUESTORE CONFIG REFERENCE](https://docs.ceph.com/en/latest/rados/configuration/bluestore-config-ref/)
 * [Ceph BlueStore](http://blog.wjin.org/posts/ceph-bluestore.html)
 * [Ceph后端存储引擎BlueStore](https://cloud-atlas.readthedocs.io/zh_CN/latest/ceph/bluestore.html)
 * [Ceph BlueStore Write Analyse](http://www.yangguanjun.com/2018/09/06/ceph-blueStore-write-analyse/)
 * [BlueStore源码分析之架构设计](https://zhuanlan.zhihu.com/p/91015613)
 * [BlueStore源码分析之对象IO](https://zhuanlan.zhihu.com/p/92397191)
