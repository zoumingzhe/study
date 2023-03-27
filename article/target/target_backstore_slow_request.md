target backstore slow request
=============================

背景
----

target使用块设备创建backstores/block，其中一个设备出现慢盘（slow request），导致所有磁盘出现io error。

信息
----

查看target进程，发现有D状态进程：

```shell
root@localhost:/# ps -aux |grep D
USER         PID %CPU %MEM    VSZ   RSS TTY      STAT START   TIME COMMAND
root         508  0.0  0.0      0     0 ?        D    04:22   0:00 [iscsi_np]
root        1178  2.7  0.0      0     0 ?        D    06:04   0:35 [kworker/0:1+events]
root        5047  4.0  0.0      0     0 ?        D    06:21   0:12 [iscsi_ttx]
root        6447  0.0  0.0   9080   132 pts/1    S+   06:26   0:00 grep --color=auto D
root@localhost:/# cat /proc/1178/stack
[<0>] target_put_cmd_and_wait+0x55/0x80
[<0>] core_tmr_abort_task.cold.10+0x15f/0x1a2
[<0>] target_tmr_work+0x96/0x110
[<0>] process_one_work+0x1e7/0x3c0
[<0>] worker_thread+0x2d/0x3c0
[<0>] kthread+0x11a/0x140
[<0>] ret_from_fork+0x22/0x30
root@localhost:/# cat /proc/508/stack
[<0>] iscsi_check_for_session_reinstatement+0x1d1/0x230
[<0>] iscsi_target_do_login+0x218/0x530
[<0>] iscsi_target_start_negotiation+0x51/0xc0
[<0>] iscsi_target_login_thread+0x635/0xdb0
[<0>] kthread+0x11a/0x140
[<0>] ret_from_fork+0x22/0x30
root@localhost:/# cat /proc/5047/stack
[<0>] __transport_wait_for_tasks+0xc2/0xf0
[<0>] transport_generic_free_cmd+0xaf/0x130
[<0>] iscsit_free_cmd+0x56/0xc0
[<0>] iscsit_close_connection+0x621/0x810
[<0>] iscsit_take_action_for_connection_exit+0x76/0xc0
[<0>] iscsi_target_tx_thread+0x17e/0x200
[<0>] kthread+0x11a/0x140
[<0>] ret_from_fork+0x22/0x30
```

调查
----

根据问题线程，首先怀疑target模块存在未修复的bug。查看社区邮件列表，发现社区近期有相关patch与堆栈高度相似，backport相关patch后继续测试，发现相关问题重现且堆栈信息相同。

手动阻塞backstore的所有IO，重复测试，记录下出现io error的用时，发现存在规律：
| start blocking io time       | io error time                | time cost |
| :--------------------------- | :--------------------------- | :-------: |
| Fri Mar 24 14:11:21 CST 2023 | Fri Mar 24 14:12:28 CST 2023 |  1min+7s  |
| Fri Mar 24 14:13:55 CST 2023 | Fri Mar 24 14:15:03 CST 2023 |  1min+8s  |
| Fri Mar 24 14:16:00 CST 2023 | Fri Mar 24 14:17:07 CST 2023 |  1min+7s  |
| Fri Mar 24 14:18:01 CST 2023 | Fri Mar 24 14:19:08 CST 2023 |  1min+7s  |
| Fri Mar 24 14:19:40 CST 2023 | Fri Mar 24 14:20:48 CST 2023 |  1min+8s  |
| Fri Mar 24 14:21:19 CST 2023 | Fri Mar 24 14:22:26 CST 2023 |  1min+7s  |
| Fri Mar 24 14:43:27 CST 2023 | Fri Mar 24 14:44:31 CST 2023 |  1min+4s  |
发现从阻塞IO到io error的耗时几乎一致，怀疑存在超时机制。因为如果是代码bug，该问题应该表现出一定的随机性。

查看dmesg发现，出现io error时打印错误信息`detected conn error (1021)`。

```text
[Fri Mar 24 15:35:26 2023]  connection3:0: detected conn error (1021)
[Fri Mar 24 15:35:26 2023]  connection3:0: detected conn error (1021)
[Fri Mar 24 15:35:29 2023]  session3: session recovery timed out after 3 secs
[Fri Mar 24 15:35:29 2023] sd 12:0:0:1: Device offlined - not ready after error recovery
[Fri Mar 24 15:35:29 2023] sd 12:0:0:1: [sds] tag#95 FAILED Result: hostbyte=DID_TRANSPORT_DISRUPTED driverbyte=DRIVER_OK
[Fri Mar 24 15:35:29 2023] sd 12:0:0:1: [sds] tag#95 CDB: Read(10) 28 00 05 93 b1 f8 00 00 08 00
```

相关错误信息打印代码如下：

```C drivers\scsi\scsi_transport_iscsi.c:iscsi_conn_error_event
void iscsi_conn_error_event(struct iscsi_cls_conn *conn, enum iscsi_err error)
{

        ......

        iscsi_cls_conn_printk(KERN_INFO, conn, "detected conn error (%d)\n",
                              error);
}
EXPORT_SYMBOL_GPL(iscsi_conn_error_event);
```

`1021`应该是错误码，查看错误码定义，`1021`代表错误`ISCSI_ERR_SCSI_EH_SESSION_RST`：

```C include\scsi\iscsi_if.h:iscsi_err
#define ISCSI_ERR_BASE                  1000

/*
 * Common error codes
 */
enum iscsi_err {
        ISCSI_OK                        = 0,

        ISCSI_ERR_DATASN                = ISCSI_ERR_BASE + 1,
        ISCSI_ERR_DATA_OFFSET           = ISCSI_ERR_BASE + 2,
        ISCSI_ERR_MAX_CMDSN             = ISCSI_ERR_BASE + 3,
        ISCSI_ERR_EXP_CMDSN             = ISCSI_ERR_BASE + 4,
        ISCSI_ERR_BAD_OPCODE            = ISCSI_ERR_BASE + 5,
        ISCSI_ERR_DATALEN               = ISCSI_ERR_BASE + 6,
        ISCSI_ERR_AHSLEN                = ISCSI_ERR_BASE + 7,
        ISCSI_ERR_PROTO                 = ISCSI_ERR_BASE + 8,
        ISCSI_ERR_LUN                   = ISCSI_ERR_BASE + 9,
        ISCSI_ERR_BAD_ITT               = ISCSI_ERR_BASE + 10,
        ISCSI_ERR_CONN_FAILED           = ISCSI_ERR_BASE + 11,
        ISCSI_ERR_R2TSN                 = ISCSI_ERR_BASE + 12,
        ISCSI_ERR_SESSION_FAILED        = ISCSI_ERR_BASE + 13,
        ISCSI_ERR_HDR_DGST              = ISCSI_ERR_BASE + 14,
        ISCSI_ERR_DATA_DGST             = ISCSI_ERR_BASE + 15,
        ISCSI_ERR_PARAM_NOT_FOUND       = ISCSI_ERR_BASE + 16,
        ISCSI_ERR_NO_SCSI_CMD           = ISCSI_ERR_BASE + 17,
        ISCSI_ERR_INVALID_HOST          = ISCSI_ERR_BASE + 18,
        ISCSI_ERR_XMIT_FAILED           = ISCSI_ERR_BASE + 19,
        ISCSI_ERR_TCP_CONN_CLOSE        = ISCSI_ERR_BASE + 20,
        ISCSI_ERR_SCSI_EH_SESSION_RST   = ISCSI_ERR_BASE + 21,
        ISCSI_ERR_NOP_TIMEDOUT          = ISCSI_ERR_BASE + 22,
};
```

而`ISCSI_ERR_SCSI_EH_SESSION_RST`错误仅在`drivers\scsi\libiscsi.c`中使用：

```C drivers\scsi\libiscsi.c:iscsi_eh_abort
int iscsi_eh_abort(struct scsi_cmnd *sc)
{
        ......
        case TMF_TIMEDOUT:
                session->running_aborted_task = task;
                spin_unlock_bh(&session->frwd_lock);
                iscsi_conn_failure(conn, ISCSI_ERR_SCSI_EH_SESSION_RST);
                goto failed_unlocked;
        ......
}
EXPORT_SYMBOL_GPL(iscsi_eh_abort);
```

```C drivers\scsi\libiscsi.c:iscsi_eh_device_reset
int iscsi_eh_device_reset(struct scsi_cmnd *sc)
{
        ......
        case TMF_TIMEDOUT:
                spin_unlock_bh(&session->frwd_lock);
                iscsi_conn_failure(conn, ISCSI_ERR_SCSI_EH_SESSION_RST);
                goto done;
        ......
}
EXPORT_SYMBOL_GPL(iscsi_eh_device_reset);
```

```C drivers\scsi\libiscsi.c:iscsi_eh_session_reset
int iscsi_eh_session_reset(struct scsi_cmnd *sc)
{
        ......

        iscsi_conn_failure(conn, ISCSI_ERR_SCSI_EH_SESSION_RST);
        iscsi_put_conn(conn->cls_conn);

        ......
}
EXPORT_SYMBOL_GPL(iscsi_eh_session_reset);
```

```C drivers\scsi\libiscsi.c:iscsi_eh_target_reset
static int iscsi_eh_target_reset(struct scsi_cmnd *sc)
{
        ......
        case TMF_TIMEDOUT:
                spin_unlock_bh(&session->frwd_lock);
                iscsi_conn_failure(conn, ISCSI_ERR_SCSI_EH_SESSION_RST);
                goto done;
        ......
}
```

这些函数均是scsi中定义的iscsi错误处理函数，所以查看scsi的错误处理函数，最终发现`Linux SCSI`层存在超时机制。

关于`Linux SCSI`层超时机制的详细介绍请浏览：[Linux SCSI Timeout](../../linux/driver/scsi/linux_scsi_timeout.md)。

结论
----

首先，使用iscsi时backstore应保证其服务质量，尽量不要出现慢盘（slow request）。
其次，backstore如果出现慢盘（slow request），可能会导致出现慢盘的traget上的所有磁盘上层io error，当慢盘（slow request）消失后io可以恢复。
最后，使用iscsi时上层scsi可以通过调整scsi盘的timeout避免频繁出现io error。
