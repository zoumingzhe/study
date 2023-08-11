workqueue crash
===============

worker_pool
-----------

```text
crash> struct -x worker_pool
struct worker_pool {
    spinlock_t lock;
    int cpu;
    int node;
    int id;
    unsigned int flags;
    unsigned long watchdog_ts;
    struct list_head worklist;
    int nr_workers;
    int nr_idle;
    struct list_head idle_list;
    struct timer_list idle_timer;
    struct timer_list mayday_timer;
    struct hlist_head busy_hash[64];
    struct worker *manager;
    struct list_head workers;
    struct completion *detach_completion;
    struct ida worker_ida;
    struct workqueue_attrs *attrs;
    struct hlist_node hash_node;
    int refcnt;
    atomic_t nr_running;
    struct callback_head rcu;
}
SIZE: 0x380
```

### cpu_worker_pools

```shell
p cpu_worker_pools
```

### worklist 和 idle_list

```text
crash> worker_pool.worklist,idle_list ffffa03ffdf7d600
  worklist = {
    next = 0xffffa03ffdf7d620,
    prev = 0xffffa03ffdf7d620
  }
  idle_list = {
    next = 0xffffa03ef255df00,
    prev = 0xffffa03ef2551900
  }
crash> worker_pool.worklist,idle_list ffffa03ffdf7d980
  worklist = {
    next = 0xffffa03ffdf7d9a0,
    prev = 0xffffa03ffdf7d9a0
  }
  idle_list = {
    next = 0xffffa03fc7e97f00,
    prev = 0xffffa03fc334e700
  }
```

worker
------

workqueue
---------

```text
crash> struct -x workqueue_struct
struct workqueue_struct {
    struct list_head pwqs;
    struct list_head list;
    struct mutex mutex;
    int work_color;
    int flush_color;
    atomic_t nr_pwqs_to_flush;
    struct wq_flusher *first_flusher;
    struct list_head flusher_queue;
    struct list_head flusher_overflow;
    struct list_head maydays;
    struct worker *rescuer;
    int nr_drainers;
    int saved_max_active;
    struct workqueue_attrs *unbound_attrs;
    struct pool_workqueue *dfl_pwq;
    struct wq_device *wq_dev;
    char name[24];
    struct callback_head rcu;
    unsigned int flags;
    struct pool_workqueue *cpu_pwqs;
    struct pool_workqueue *numa_pwq_tbl[];
}
SIZE: 0x140
```

work
----

```text
crash> struct -x work_struct
struct work_struct {
    atomic_long_t data;
    struct list_head entry;
    work_func_t func;
    unsigned long rh_reserved1;
    unsigned long rh_reserved2;
    unsigned long rh_reserved3;
    unsigned long rh_reserved4;
}
SIZE: 0x40
```
