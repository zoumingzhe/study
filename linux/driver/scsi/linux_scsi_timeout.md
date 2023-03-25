Linux SCSI Timeout
==================

Linux SCSI层为每个命令设置一个定时器。当此定时器过期时，SCSI层将静默主机总线适配器（HBA），并等待所有未完成的命令超时或完成。之后，SCSI层将激活驱动程序的错误处理程序。

触发错误处理程序时，它会按顺序尝试以下操作（直到某一操作执行成功）：

1. Abort the command.
2. Reset the device.
3. Reset the target.
4. Reset the bus.
5. Reset the host.

```C drivers\scsi\scsi_error.c:scsi_abort_eh_cmnd
static void scsi_abort_eh_cmnd(struct scsi_cmnd *scmd)
{
        if (scsi_try_to_abort_cmd(scmd->device->host->hostt, scmd) != SUCCESS)
                if (scsi_try_bus_device_reset(scmd) != SUCCESS)
                        if (scsi_try_target_reset(scmd) != SUCCESS)
                                if (scsi_try_bus_reset(scmd) != SUCCESS)
                                        scsi_try_host_reset(scmd);
}
```

如果所有这些操作都失败，设备将设置为`offline`状态。发生这种情况时，到该设备的所有IO都将失败，直到问题得到纠正并且用户将设备设置为`running`状态。

但是，如果设备使用`fibre channel`协议并且`rport`被阻塞，则过程会有所不同。在这种情况下，驱动程序会等待几秒钟，让`rport`再次联机，然后再激活错误处理程序。这可以防止设备由于临时传输问题而脱机。

设备状态
--------

若要查看设备的状态，请使用：

```shell
cat /sys/block/<device-name>/device/state
```

若要将设备设置为`running`状态，请使用：

```shell
echo running > /sys/block/<device-name>/device/state
```

命令定时器
----------

要控制命令定时器，可以写入`/sys/block/device-name/device/timeout`。为此，请运行：

```shell
echo <value> > /sys/block/<device-name>/device/timeout
```

此处，`value`是您要求的超时值（单位秒）。

或者，您也可以修改`udev`的超时规则。为此，请打开`/etc/udev/rules.d/50-udev.rules`。您应该找到以下行：

```text
ACTION=="add", SUBSYSTEM=="scsi" , SYSFS{type}=="0|7|14", \
RUN+="/bin/sh -c 'echo 60 > /sys$$DEVPATH/timeout'"
```

`echo 60`指超时时长，单位秒；在这种情况下，超时设置为60秒。将此值替换为您想要的超时长度。

请注意，使用`udev`时，普通文件系统命令的默认超时为60秒。如果未使用`udev`，则默认超时为30秒。

参考
----

* [Chapter 40. Controlling the SCSI Command Timer and Device Status Red Hat Enterprise Linux 6 | Red Hat Customer Portal](https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/6/html/storage_administration_guide/scsi-command-timer-device-status#doc-wrapper)
* [Setting the SCSI command timeout](https://www.ibm.com/docs/en/linux-on-systems?topic=wsd-setting-command-timeout)
