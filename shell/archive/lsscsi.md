lsscsi
======
Usage: lsscsi   [--classic] [--device] [--generic] [--help] [--hosts]
		[--kname] [--list] [--lunhex] [--long] [--protection]
		[--scsi_id] [--size] [--sysfsroot=PATH] [--transport]
		[--verbose] [--version] [--wwn] [<h:c:t:l>

List SCSI devices or hosts, optionally with additional information.

lsscsi命令用于显示 SCSI 设备或主机。

Options:
    --classic|-c      alternate output similar to 'cat /proc/scsi/scsi'
    --device|-d       show device node's major + minor numbers
    --generic|-g      show scsi generic device name
    --help|-h         this usage information
    --hosts|-H        lists scsi hosts rather than scsi devices
    --kname|-k        show kernel name instead of device node name
    --list|-L         additional information output one
                      attribute=value per line
    --long|-l         additional information output
    --lunhex|-x       show LUN part of tuple as hex number in T10 format;
                      use twice to get full 16 digit hexadecimal LUN
    --protection|-p   show target and initiator protection information
    --protmode|-P     show negotiated protection information mode
    --scsi_id|-i      show udev derived /dev/disk/by-id/scsi* entry
    --size|-s         show disk size
    --sysfsroot=PATH|-y PATH    set sysfs mount point to PATH (def: /sys)
    --transport|-t    transport information for target or, if '--hosts'
                      given, for initiator
    --unit|-u         logical unit (LU) name (aka WWN for ATA/SATA)
    --verbose|-v      output path names where data is found
    --version|-V      output version string and exit
    --wwn|-w          output WWN for disks (from /dev/disk/by-id/wwn*)
    <h:c:t:l>         filter output list (def: '*:*:*:*' (all))


# 参考
 * []()
