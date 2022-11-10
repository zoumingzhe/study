# bio


# bio的拆分（split）


# bio的合并（merge）
单个bio处理的数据大小是有限制的，这些限制参数存放在struct queue_limits结构体中。
当bio处理的数据大小超出限制，这个bio就会被拆分成多个满足条件的bio，并组成一个chain，然后依次处理这些bio，直至chain中的所有bio处理完。





# 参考
 * [BLOCK 层这么多参数都是什么意思？！](https://developer.aliyun.com/article/784610)