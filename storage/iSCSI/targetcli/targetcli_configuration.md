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
 - bool: Boolean. Values: {TRUE|FALSE}
 - bool-iSCSI: Boolean per RFC 3720. Values: {Yes|No}
 - enum: List of alphanumeric values.
 - integer: Digits. Values: [0–9]
 - string: Alphanumeric characters
 - color: {black|blue|cyan|default|green|magenta|red|white|yellow}
 - loglevel: {critical|debug|error|info|warning}

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
##### /backstores/block/\<name>/alua
###### /backstores/block/\<name>/alua/default_tg_pt_gp
**alua config group**

## /iscsi
**discovery_auth config group**
如果启用了发现身份验证，则initiator必须通过发送匹配的password和userid登录凭据来提供单向身份验证。如果也设置了mutual_userid和mutual_password，则target必须通过其登录凭据进行响应以相互身份验证。

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
