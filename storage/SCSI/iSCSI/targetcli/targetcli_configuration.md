targetcli configuration
=======================

# config group
| path                                            | global | discovery_auth | attribute | auth | parameter | alua |
|:----------------------------------------------- |:------:|:--------------:|:---------:|:----:|:---------:|:----:|
| /                                               |   √    |                |           |      |           |      |
| /backstores                                     |   √    |                |           |      |           |      |
| /backstores/block                               |   √    |                |           |      |           |      |
| /backstores/block/\<name>                       |   √    |                |     √     |      |           |      |
| /backstores/block/\<name>/alua                  |   √    |                |           |      |           |      |
| /backstores/block/\<name>/alua/default_tg_pt_gp |   √    |                |           |      |           |  √   |
| /backstores/fileio                              |   √    |                |           |      |           |      |
| /backstores/pscsi                               |   √    |                |           |      |           |      |
| /backstores/ramdisk                             |   √    |                |           |      |           |      |
| /iscsi                                          |   √    |       √        |           |      |           |      |
| /iscsi/\<wwn>                                   |   √    |                |           |      |           |      |
| /iscsi/\<wwn>/tpg1                              |   √    |                |     √     |  √   |     √     |      |
| /iscsi/\<wwn>/tpg1/acls                         |   √    |                |           |      |           |      |
| /iscsi/\<wwn>/tpg1/acls/\<wwn>                  |   √    |                |     √     |  √   |     √     |      |
| /iscsi/\<wwn>/tpg1/acls/\<wwn>/mapped_lun\<n>   |   √    |                |           |      |           |      |
| /iscsi/\<wwn>/tpg1/luns                         |   √    |                |           |      |           |      |
| /iscsi/\<wwn>/tpg1/luns/lun\<n>                 |   √    |                |           |      |           |  √   |
| /iscsi/\<wwn>/tpg1/portals                      |   √    |                |           |      |           |      |
| /iscsi/\<wwn>/tpg1/portals/ip:port              |   √    |                |           |      |           |      |
| /loopback                                       |   √    |                |           |      |           |      |

# variable types
The following variable types are used for targetcli command parameters and attributes:
 - bool: Boolean. Values: `{TRUE|FALSE}`
 - bool-iSCSI: Boolean per RFC 3720. Values: `{Yes|No}`
 - enum: List of alphanumeric values.
 - integer: Digits. Values: `[0–9]`
 - string: Alphanumeric characters
 - color: `{black|blue|cyan|default|green|magenta|red|white|yellow}`
 - loglevel: `{critical|debug|error|info|warning}`

# global config group
 - auto_add_default_portal
If true, adds a portal listening on all IPs to new targets.

 - auto_add_mapped_luns = \<bool>
If true, automatically create node ACLs mapped LUNs after creating a new target LUN or a new node ACL.

 - auto_cd_after_create = \<bool>
If true, changes current path to newly created objects.

 - auto_enable_tpgt = \<bool>
If true, automatically enables TPGTs upon creation.

 - auto_save_on_exit
If true, saves configuration on exit.

 - auto_use_daemon
If true, commands will be sent to targetclid.

 - color_command = \<color>
Color to use for command completions.

 - color_default = \<color>
Default text display color.

 - color_keyword = \<color>
Color to use for keyword completions.

 - color_mode = \<bool>
Console color display mode.

 - color_parameter = \<color>
Color to use for parameter completions.

 - color_path = \<color>
Color to use for path completions.

 - daemon_use_batch_mode
If true, use batch mode for daemonized approach.

 - export_backstore_name_as_model
If true, the backstore name is used for the scsi inquiry model name.

 - logfile = \<string>
Logfile to use.

 - loglevel_console = \<loglevel>
Log level for messages going to the console.

 - loglevel_file = \<loglevel>
Log level for messages going to the log file.

 - max_backup_files
Max no. of configurations to be backed up in /etc/target/backup/ directory.

 - prompt_length = \<integer>
Max length of the shell prompt path, 0 for infinite.

 - tree_max_depth = \<integer>
Maximum depth of displayed node tree.

 - tree_round_nodes = \<bool>
Tree node display style.

 - tree_show_root = \<bool>
Whether or not to display tree root.

 - tree_status_mode = \<bool>
Whether or not to display status in tree.

# /
## /backstores
### /backstores/block
#### /backstores/block/\<name>
**attribute config group**
 - alua_support=1 [ro]


 - block_size=512
Block size of the underlying device.

 - emulate_3pc=1
If set to 1, enable Third Party Copy.

 - emulate_caw=1
If set to 1, enable Compare and Write.

 - emulate_dpo=1
If set to 1, turn on Disable Page Out.

 - emulate_fua_read=1
If set to 1, enable Force Unit Access read.

 - emulate_fua_write=1
If set to 1, enable Force Unit Access write.

 - emulate_model_alias=1
If set to 1, use the backend device name for the model alias.

 - emulate_pr=1
If set to 1, enable SCSI Reservations.

 - emulate_rest_reord=0
If set to 0, the Queue Algorithm Modifier is Restricted Reordering.

 - emulate_tas=1
If set to 1, enable Task Aborted Status.

 - emulate_tpu=0
If set to 1, enable Thin Provisioning Unmap.

 - emulate_tpws=0
If set to 1, enable Thin Provisioning Write Same.

 - emulate_ua_intlck_ctrl=0
If set to 1, enable Unit Attention Interlock.

 - emulate_write_cache=0
If set to 1, turn on Write Cache Enable.

 - enforce_pr_isids=1
If set to 1, enforce persistent reservation ISIDs.

 - force_pr_aptpl=0
If set to 1, force SPC-3 PR Activate Persistence across Target Power Loss operation.

 - hw_block_size=512 [ro]
Hardware block size in bytes.

 - hw_max_sectors=131072 [ro]
Maximum number of sectors the hardware can transfer at once.

 - hw_pi_prot_type=0 [ro]
If non-zero, DIF protection is enabled on the underlying hardware.

 - hw_queue_depth=256 [ro]
Hardware queue depth.

 - is_nonrot=1
If set to 1, the backstore is a non rotational device.

 - max_unmap_block_desc_count=1
Maximum number of block descriptors for UNMAP.

 - max_unmap_lba_count=131072
Maximum number of LBA for UNMAP.

 - max_write_same_len=131072
Maximum length for WRITE_SAME.

 - optimal_sectors=131072
Optimal request size in sectors.

 - pgr_support=1 [ro]


 - pi_prot_format=0
DIF protection format.

 - pi_prot_type=0
DIF protection type.

 - pi_prot_verify=0


 - queue_depth=256
Queue depth.

 - unmap_granularity=131072
UNMAP granularity.

 - unmap_granularity_alignment=0
UNMAP granularity alignment.

 - unmap_zeroes_data=131072
If set to 1, zeroes are read back after an UNMAP.

##### /backstores/block/\<name>/alua
###### /backstores/block/\<name>/alua/default_tg_pt_gp
**alua config group**
 - alua_access_state=0
The alua_access_state alua parameter.

 - alua_access_status=0
The alua_access_status alua parameter.

 - alua_access_type=3
The alua_access_type alua parameter.

 - alua_support_active_nonoptimized=1
The alua_support_active_nonoptimized alua parameter.

 - alua_support_active_optimized=1
The alua_support_active_optimized alua parameter.

 - alua_support_lba_dependent=0 [ro]
The alua_support_lba_dependent alua parameter.

 - alua_support_offline=1
The alua_support_offline alua parameter.

 - alua_support_standby=1
The alua_support_standby alua parameter.

 - alua_support_transitioning=1
The alua_support_transitioning alua parameter.

 - alua_support_unavailable=1
The alua_support_unavailable alua parameter.

alua_write_metadata=0
The alua_write_metadata alua parameter.

 - implicit_trans_secs=0
The implicit_trans_secs alua parameter.

 - members=[{'tpgt': 1, 'driver': 'iSCSI', 'target': 'iqn.2003-01.org.linux-iscsi.node-1.x8664:sn.956ba6728c8c', 'lun': 0] [ro]
The members alua parameter.

 - nonop_delay_msecs=100
The nonop_delay_msecs alua parameter.

 - preferred=0
The preferred alua parameter.

 - tg_pt_gp_id=0 [ro]
The tg_pt_gp_id alua parameter.

 - trans_delay_msecs=0
The trans_delay_msecs alua parameter.

## /iscsi
**discovery_auth config group**
如果启用了发现身份验证，则initiator必须通过发送匹配的password和userid登录凭据来提供单向身份验证。如果也设置了mutual_userid和mutual_password，则target必须通过其登录凭据进行响应以双向身份验证。

 - enable = \<bool>
The enable discovery_auth parameter.

 - mutual_password = \<string>
The mutual_password discovery_auth parameter.

 - mutual_userid = \<string>
The mutual_userid discovery_auth parameter.

 - password = \<string>
The password discovery_auth parameter.

 - userid = \<string>
The userid discovery_auth parameter.

### /iscsi/\<wwn>
#### /iscsi/\<wwn>/tpg1
**auth config group**
 - mutual_password = \<string>
The mutual_password auth parameter.

 - mutual_userid = \<string>
The mutual_userid auth parameter.

 - password = \<string>
The password auth parameter.

 - userid = \<string>
The userid auth parameter.

**attribute config group**
 - authentication = \<bool>
If set to 1, enforce authentication for this TPG.

 - cache_dynamic_acls = \<bool>
If set to 1 in demo mode, cache dynamically generated ACLs.

 - default_cmdsn_depth = \<integer>
Default CmdSN (Command Sequence Number) depth.
建议1GbE网络使用16，10GbE网络使用64。

 - default_erl
Default Error Recovery Level.

 - demo_mode_discovery
If set to 1 in demo mode, enable discovery.

 - demo_mode_write_protect = \<bool>
If set to 1 in demo mode, prevent writes to LUNs.

 - fabric_prot_type
Fabric DIF protection type.

 - generate_node_acls = \<bool>
If set to 1, allow all initiators to login (i.e. demo mode).

 - login_keys_workaround=1


 - login_timeout = \<integer>
Login timeout value in seconds.

 - netif_timeout
NIC failure timeout in seconds.

 - prod_mode_write_protect = \<bool>
If set to 1, prevent writes to LUNs.

 - t10_pi=0
--------
If set to 1, enable T10 Protection Information.

 - tpg_enabled_sendtargets
If set to 1, the SendTargets discovery response advertises the TPG only if the TPG is enabled.

**parameter config group**
 - AuthMethod = \<enum>
Authentication method used by the TPG.

 - DataDigest = \<enum>
If set to CRC32C, the integrity of the PDU data part is verified.

 - DataPDUInOrder = \<bool-iSCSI>
If set to Yes, the data PDUs within sequences must be in order.

 - DataSequenceInOrder = \<bool-iSCSI>
If set to Yes, the data sequences must be in order.

 - DefaultTime2Retain = \<integer>
Maximum time, in seconds, after an initial wait, before which an active task reassignment is still possible after an unexpected connection termination or a connection reset.

 - DefaultTime2Wait = \<integer>
Minimum time, in seconds, to wait before attempting an explicit/implicit logout or an active task reassignment after an unexpected connection termination or a connection reset.

 - ErrorRecoveryLevel = \<integer>
Recovery levels represent a combination of recovery capabilities.

 - FirstBurstLength = \<integer>
Maximum amount in bytes of unsolicited data an initiator may send.

 - HeaderDigest = \<enum>
If set to CRC32C, the integrity of the PDU header part is verified.

 - IFMarkInt=Reject
Deprecated according to RFC 7143.

 - IFMarker = \<bool-iSCSI>
Deprecated according to RFC 7143.

 - ImmediateData = \<bool-iSCSI>
Immediate data support.

 - InitialR2T = \<bool-iSCSI>
If set to No, the default use of R2T (Ready To Transfer) is disabled.

 - MaxBurstLength = \<integer>
Maximum SCSI data payload in bytes in a Data-In or a solicited Data-Out iSCSI sequence.

 - MaxConnections = \<integer>
Maximum number of connections acceptable.

 - MaxOutstandingR2T = \<integer>
Maximum number of outstanding R2Ts per task.

 - MaxRecvDataSegmentLength = \<integer>
Maximum data segment length in bytes the target can receive in an iSCSI PDU.

 - MaxXmitDataSegmentLength = \<integer>
Outgoing MaxRecvDataSegmentLength sent over the wire during iSCSI login response.

 - OFMarkInt=Reject
Deprecated according to RFC 7143.

 - OFMarker = \<bool-iSCSI>
Deprecated according to RFC 7143.

 - TargetAlias = \<string>
Human-readable target name or description.

##### /iscsi/\<wwn>/tpg1/acls
###### /iscsi/\<wwn>/tpg1/acls/\<wwn>
**auth config group**
 - mutual_password = \<string>
The mutual_password auth parameter.

 - mutual_userid = \<string>
The mutual_userid auth parameter.

 - password = \<string>
The password auth parameter.

 - userid = \<string>
The userid auth parameter.

**attribute config group**
 - dataout_timeout = \<integer>
Data-Out timeout in seconds before invoking recovery.

 - dataout_timeout_retries = \<integer>
Number of Data-Out timeout recovery attempts before failing a path.

 - default_erl = \<integer>
Default Error Recovery Level.

 - nopin_response_timeout = \<integer>
Nop-In response timeout in seconds.

 - nopin_timeout = \<integer>
Nop-In timeout in seconds.

 - random_datain_pdu_offsets = \<bool>
If set to 1, request random Data-In PDU offsets.

 - random_datain_seq_offsets = \<bool>
If set to 1, request random Data-In sequence offsets.

 - random_r2t_offsets = \<bool>
If set to 1, request random R2T (Ready To Transfer) offsets.

**parameter config group**
 - DataPDUInOrder [ro]
If set to Yes, the data PDUs within sequences must be in order.

 - DataSequenceInOrder [ro]
If set to Yes, the data sequences must be in order.

 - DefaultTime2Retain [ro]
Maximum time, in seconds, after an initial wait, before which an active task reassignment is still possible after an unexpected connection termination or a connection reset.

 - DefaultTime2Wait [ro]
Minimum time, in seconds, to wait before attempting an explicit/implicit logout or an active task reassignment after an unexpected connection termination or a connection reset.

 - ErrorRecoveryLevel [ro]
Recovery levels represent a combination of recovery capabilities.

 - FirstBurstLength [ro]
Maximum amount in bytes of unsolicited data an initiator may send.

 - ImmediateData [ro]
Immediate data support.

 - InitialR2T [ro]
If set to No, the default use of R2T (Ready To Transfer) is disabled.

 - MaxBurstLength [ro]
Maximum SCSI data payload in bytes in a Data-In or a solicited Data-Out iSCSI sequence.

 - MaxConnections [ro]
Maximum number of connections acceptable.

 - MaxOutstandingR2T [ro]
Maximum number of outstanding R2Ts per task.

##### /iscsi/\<wwn>/tpg1/luns
###### /iscsi/\<wwn>/tpg1/luns/lun\<n>
**alua config group**
 - alua_tg_pt_gp_name = \<string>
The alua_tg_pt_gp_name alua parameter.

##### /iscsi/\<wwn>/tpg1/portals

## /loopback

# 参考
 * [LIO Admin Manual](http://www.linux-iscsi.org/Doc/LIO%20Admin%20Manual.pdf)
