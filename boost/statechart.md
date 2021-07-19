boost::statechart
=================
boost包含两个独立的状态机库：Statechart和Meta State Machine（MSM）。
 - Boost.Statechart：可以用易于阅读和维护的C++代码实现任意复杂的有限状态机。
 - MSM元状态机：用于表达性UML2有限状态机的高性能库。

boost::statechart主要分为三部分：
 1. 状态机（statemachine）：事件处理机。
 2. 状态（state）：可以接收事件，在此状态类重载的react()函数中进行事件处理，或者根据接收的事件进行状态跳转。
 3. 事件（event）：事件可以改变状态，促使状态发生转移。

# 参考
 * [boost::statechart学习记录](https://blog.csdn.net/yang06lei11/article/details/99731972)
 * [Boost Statechart vs. Meta State Machine](https://stackoverflow.com/questions/4275602/boost-statechart-vs-meta-state-machine)
 * [一个Boost Statechart Library有限状态机实用示例](https://blog.csdn.net/CharmingSun/article/details/105486342)
