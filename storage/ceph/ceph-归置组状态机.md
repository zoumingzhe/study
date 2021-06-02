Ceph Placement Group State Machine （归置组状态机）
==================================================
Ceph在处理PG的状态转换时，使用了boost库提供的[statechart](../../boost/statechart.md)状态机。

从Octopus（v15.1.0）版本开始，这部分代码由/src/osd/PG.h迁至./src/osd/PeeringState.h，名称由RecoveryMachine变为PeeringMachine。

## 利用`gen_state_diagram.py`与`Graphviz`生成状态转换图
```shell
cat ./src/osd/PeeringState.h ./src/osd/PeeringState.cc | ./doc/scripts/gen_state_diagram.py > ~/Pictures/PeeringState.dot
dot -Tpng ~/Pictures/PeeringState.dot -o ~/Pictures/PeeringState.png
```

# 参考
 * [ceph peering state model](https://bean-li.github.io/ceph-peering-state-model/)
 * [Graphviz - Graph Visualization Software](http://www.graphviz.org/)
