iSCSI 相关概念
==============

# iSCSI Initiator （iSCSI Client）
iSCSI客户端，I/O操作的启动者。

# iSCSI Target （iSCSI Server）
iSCSI服务端，I/O操作的执行者。

# iSCSI Naming
iSCSI名称是iSCSI节点的唯一标识符，有两种格式（iqn、eui），最常见的是iqn格式。

更多有关iSCSI命名的详细信息，请参考IETF的[rfc3721](https://datatracker.ietf.org/doc/html/rfc3721)和[rfc3722](https://datatracker.ietf.org/doc/html/rfc3722)。

## IQN （iSCSI Qualified Name）
iqn格式：**iqn.yyyy-mm.naming_authority:unique_name**
 - *yyyy-mm*是命名机构成立的年份和月份。
 - *naming_authority*是命名机构Internet域名的逆转格式。
 - *unique_name*是命名机构必须确保唯一的任何名称。

## EUI （Enterprise Unique Identifier）
eui格式：**eui.16_hex_digits**

16位十六进制数字是IEEE EUI（扩展唯一标识符）格式的64位数字的文本表示。前24位是IEEE向特定公司注册的公司ID，剩余的40位由持有该公司ID的实体分配，并且必须是唯一的。

# iSCSI TPG
# iSCSI acl
# iSCSI portals


# LUN （Logical Unit Number）


# RFC
 * [rfc3720](https://datatracker.ietf.org/doc/html/rfc3720)
 * [rfc3721](https://datatracker.ietf.org/doc/html/rfc3721)
 * [rfc3722](https://datatracker.ietf.org/doc/html/rfc3722)
 * [rfc3980](https://datatracker.ietf.org/doc/html/rfc3980)
 * [rfc4850](https://datatracker.ietf.org/doc/html/rfc4850)
 * [rfc5046](https://datatracker.ietf.org/doc/html/rfc5046)
 * [rfc5047](https://datatracker.ietf.org/doc/html/rfc5047)
 * [rfc5048](https://datatracker.ietf.org/doc/html/rfc5048)
 * [rfc7143](https://datatracker.ietf.org/doc/html/rfc7143)
 * [rfc7144](https://datatracker.ietf.org/doc/html/rfc7144)
 * [rfc7145](https://datatracker.ietf.org/doc/html/rfc7145)

# 参考
 * [常见的三种存储技术以及iSCSI协议](https://blog.51cto.com/xslwahaha/1617781)
 * [iSCSI Naming Conventions](https://docs.vmware.com/en/VMware-vSphere/7.0/com.vmware.vsphere.storage.doc/GUID-686D92B6-A2B2-4944-8718-F1B74F6A2C53.html)
