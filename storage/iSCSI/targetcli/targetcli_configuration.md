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
## auto_add_default_portal
If true, adds a portal listening on all IPs to new targets.

## auto_add_mapped_luns = <bool>
Default value: TRUE
If true, automatically create node ACLs mapped LUNs after creating a new target LUN or a new node ACL.

## auto_cd_after_create = <bool>
Default value: TRUE
If true, changes current path to newly created objects.

## auto_enable_tpgt = <bool>
Default value: TRUE
If true, automatically enables TPGTs upon creation.

## auto_save_on_exit
If true, saves configuration on exit.

## auto_use_daemon
If true, commands will be sent to targetclid.

## color_command = <color>
Default value: cyan
Color to use for command completions.

## color_default = <color>
Default value: none
Default text display color.

## color_keyword = <color>
Default value: cyan
Color to use for keyword completions.

## color_mode = <bool>
Default value: TRUE
Console color display mode.

## color_parameter = <color>
Default value: magenta
Color to use for parameter completions.

## color_path = <color>
Default value: magenta
Color to use for path completions

## daemon_use_batch_mode
If true, use batch mode for daemonized approach.

## export_backstore_name_as_model
If true, the backstore name is used for the scsi inquiry model name.

## logfile = <string>
Default value: “~/.targetcli/log.tst”
Logfile to use.

## loglevel_console = <loglevel>
Default value: info
Log level for messages going to the console.

## loglevel_file = <loglevel>
Default value: debug
Log level for messages going to the log file.

## max_backup_files
Max no. of configurations to be backed up in /etc/target/backup/ directory.

## prompt_length = <integer>
Default value: 30
Max length of the shell prompt path, 0 for infinite.

## tree_max_depth = <integer>
Default value: 0
Maximum depth of displayed node tree.

## tree_round_nodes = <bool>
Default value: TRUE
Tree node display style.

## tree_show_root = <bool>
Default value: TRUE
Whether or not to display tree root.

## tree_status_mode = <bool>
Default value: TRUE
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

### /iscsi/\<wwn>
#### /iscsi/\<wwn>/tpg1
**attribute config group**
**auth config group**
**parameter config group**
##### /iscsi/\<wwn>/tpg1/acls
###### /iscsi/\<wwn>/tpg1/acls/\<wwn>
**attribute config group**
**auth config group**
**parameter config group**
##### /iscsi/\<wwn>/tpg1/luns
###### /iscsi/\<wwn>/tpg1/luns/lun\<n>
**alua config group**

##### /iscsi/\<wwn>/tpg1/portals

## /loopback

