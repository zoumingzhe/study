Ceph RBD 命令解析
================

# rbd ls
`rbd ls [<poolname>]`
查看POOL内的RBD镜像列表

# rbd info
`rbd info <image_name>`

# rbd create
`rbd create <rbdname> -p <poolname> --size <int[M/G/T]>`
创建RBD

# rbd rename
`rbd rename <image_name> <image_name>`

# rbd resize
`rbd resize <image_name> --size <size>`

# 参考
 * []()
