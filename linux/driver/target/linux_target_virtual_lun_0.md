Linux Target virtual LUN 0
==========================

LUN 0，是小型计算机系统接口（[SCSI](../../../storage-%E5%AD%98%E5%82%A8/SCSI/)）协议要求的，需要存在才能与驱动程序正确通信。

对于`Linux Target`，所有非`Fibre Channel`协议的`TPG`创建时就分配了`tpg_virt_lun0`：

```C include\scsi\scsi_proto.h:scsi_protocol
/* SCSI protocols; these are taken from SPC-3 section 7.5 */
enum scsi_protocol {
        SCSI_PROTOCOL_FCP = 0,  /* Fibre Channel */
        SCSI_PROTOCOL_SPI = 1,  /* parallel SCSI */
        SCSI_PROTOCOL_SSA = 2,  /* Serial Storage Architecture - Obsolete */
        SCSI_PROTOCOL_SBP = 3,  /* firewire */
        SCSI_PROTOCOL_SRP = 4,  /* Infiniband RDMA */
        SCSI_PROTOCOL_ISCSI = 5,
        SCSI_PROTOCOL_SAS = 6,
        SCSI_PROTOCOL_ADT = 7,  /* Media Changers */
        SCSI_PROTOCOL_ATA = 8,
        SCSI_PROTOCOL_UNSPEC = 0xf, /* No specific protocol */
};
```

```C drivers\target\target_core_tpg.c:core_tpg_register
int core_tpg_register(
        struct se_wwn *se_wwn,
        struct se_portal_group *se_tpg,
        int proto_id)
{
        ......

        if (se_tpg->proto_id >= 0) {
                se_tpg->tpg_virt_lun0 = core_tpg_alloc_lun(se_tpg, 0);
                if (IS_ERR(se_tpg->tpg_virt_lun0))
                        return PTR_ERR(se_tpg->tpg_virt_lun0);

                ret = core_tpg_add_lun(se_tpg, se_tpg->tpg_virt_lun0,
                                true, g_lun0_dev);
                if (ret < 0) {
                        kfree(se_tpg->tpg_virt_lun0);
                        return ret;
                }
        }

        ......

        return 0;
}
EXPORT_SYMBOL(core_tpg_register);
```

直到`TPG`被删除时才会释放`tpg_virt_lun0`：

```C drivers\target\target_core_tpg.c:core_tpg_deregister
int core_tpg_deregister(struct se_portal_group *se_tpg)
{
        ......

        if (se_tpg->proto_id >= 0) {
                core_tpg_remove_lun(se_tpg, se_tpg->tpg_virt_lun0);
                kfree_rcu(se_tpg->tpg_virt_lun0, rcu_head);
        }

        return 0;
}
EXPORT_SYMBOL(core_tpg_deregister);
```

Force WRITE PROTECT for virtual LUN 0
-------------------------------------

当访问`MappedLUN=0`不存在时，target会为`virtual LUN 0`强制写保护，返回`TCM_WRITE_PROTECTED`错误，而非`TCM_NON_EXISTENT_LUN`。

```C drivers\target\target_core_device.c:transport_lookup_cmd_lun
sense_reason_t
transport_lookup_cmd_lun(struct se_cmd *se_cmd)
{
        ......

        if (!se_lun) {
                /*
                 * Use the se_portal_group->tpg_virt_lun0 to allow for
                 * REPORT_LUNS, et al to be returned when no active
                 * MappedLUN=0 exists for this Initiator Port.
                 */
                if (se_cmd->orig_fe_lun != 0) {
                        pr_err("TARGET_CORE[%s]: Detected NON_EXISTENT_LUN"
                                " Access for 0x%08llx from %s\n",
                                se_cmd->se_tfo->fabric_name,
                                se_cmd->orig_fe_lun,
                                nacl->initiatorname);
                        return TCM_NON_EXISTENT_LUN;
                }

                /*
                 * Force WRITE PROTECT for virtual LUN 0
                 */
                if ((se_cmd->data_direction != DMA_FROM_DEVICE) &&
                    (se_cmd->data_direction != DMA_NONE))
                        return TCM_WRITE_PROTECTED;

                se_lun = se_sess->se_tpg->tpg_virt_lun0;
                if (!percpu_ref_tryget_live(&se_lun->lun_ref))
                        return TCM_NON_EXISTENT_LUN;

                se_cmd->se_lun = se_sess->se_tpg->tpg_virt_lun0;
                se_cmd->se_cmd_flags |= SCF_SE_LUN_CMD;
                se_cmd->lun_ref_active = true;
        }

        ......
}
```

对于多路径`multipath`，`TCM_NON_EXISTENT_LUN`错误会将当前路径标记为`faulty`并在其他路径重发，而`TCM_WRITE_PROTECTED`错误则会直接引起`IO error`。

**强烈建议：不要使用`MappedLUN=0`！**

参考
----

* [Logical Unit Number(LUN) 0, a SCSI Requirement](https://support.oracle.com/knowledge/Sun%20Microsystems/1017707_1.html)
