Ceph ObjectStore
================
所有的Ceph前端提供的存储服务：`块设备（Ceph Block Devices）`、`对象存储（Ceph Object Storage）`、`文件系统（Ceph File System）`最后都会转化成后端的ObjectStore存储引擎，其代码实现在`./src/os`中。ObjectStore是Ceph OSD中最重要的概念之一，它封装了所有对底层存储的IO操作。

目前，Ceph实现了几种ObjectStore存储引擎：
 - FileStore：基于文件系统保存数据和元数据，LevelDB保存元数据。
 - BlueStore：基于裸盘保存数据，RocksDB保存元数据。
 - KStore：基于KV。
 - MemStore：基于内存保存数据和元数据，用以测试和验证。


object包含3个元素data，xattr，omap。data是保存对象的数据，xattr是保存对象的扩展属性，每个对象文件都可以设置文件的属性，这个属性是一个key/value值对，但是受到文件系统的限制，key/value对的个数和每个value的大小都进行了限制。如果要设置的对象的key/value不能存储在文件的扩展属性中，还存在另外一种方式保存omap，omap实际上是保存到了key/vaule  值对的数据库levelDB中，在这里value的值限制要比xattr中好的多。



ObjectStore主要接口分为三部分：
 1. Object的读写操作，类似于POSIX的部分接口。
 2. Object的属性(xattr)读写操作，这类操作的特征是kv对并且与某一个Object关联。
 3. Object的kv操作（在Ceph中称为omap），这个其实与属性(xattr)非常类似，但是在实现上可能会有所变化。

# 参考
 * [Ceph存储后端ObjectStore架构和技术演进](https://cloud.tencent.com/developer/news/300032)
 * [解析Ceph: 存储引擎实现之一–FileStore](https://www.talkwithtrend.com/Article/176745)
 * [ceph本地对象存储(2)](https://ivanzz1001.github.io/records/post/ceph/2019/01/05/ceph-src-code-part7_2)
