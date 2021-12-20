iSCSI 访问控制
==============
iSCSI访问控制包括：[Targetcli Discovery Control](./targetcli/targetcli_discovery_control.md)和[Targetcli Access Control](./targetcli/targetcli_access_control.md)。
 - Discovery Control是iSCSI Target对iSCSI Initiator的探索身份验证，有单向CHAP、双向CHAP两种身份验证配置。
 - Access Control是iSCSI Target对iSCSI Initiator的LUN访问身份验证，有demo mode、单向CHAP、双向CHAP三种身份验证配置。

当iSCSI Initiator需要访问配置了身份验证的iSCSI Target时，可通过[iscsid](./iscsid/iscsid_chap_settings.md)进行相关配置。

# targetcli启用身份验证
## TPG启用身份验证
`/iscsi/<IQN>/<TPG>> set attribute authentication=1`

**警告：设置为0时将禁用身份验证，这会导致ACLs中任一客户端（initiator）iqn都可以访问target而无需验证密码。**

# TPG身份验证方式
`/iscsi/<IQN>/<TPG>> set parameter AuthMethod=CHAP`

# Discovery Control
## CHAP Initiator Discovery Authentication
 - 设置iscsi下的discovery_auth，对所有initiator均有效。
 - 仅target对initiator单向身份验证。

## Mutual CHAP Discovery Authentication
 - 设置iscsi下的discovery_auth，对所有initiator均有效。
 - target和initiator双向身份验证。

# Access Control
## demo mode
 - 设置TPG下的attribute。
 - 无身份验证。

## CHAP Initiator Authentication
 - 设置每个ACL下的auth，仅ACL下有效。
 - 仅target对initiator单向身份验证。

## Mutual CHAP Authentication
 - 设置每个ACL下的auth，仅ACL下有效。
 - target和initiator双向身份验证。

## TPG Authentication
 - 单独为每个initiator设置身份验证信息可能会很麻烦，因此可为TPG的所有initiator设置相同的身份验证。
 - 设置TPG下的auth，仅对TPG下ACL的auth空缺有效，若ACL的auth被设置则ACL下的auth有效。
 - 可设置仅target对initiator身份验证或者target和initiator双向身份验证。

# 参考
 * [Linux iSCSI CHAP 身分驗證](http://benjr.tw/15591)
 * [Linux iSCSI initiator](http://benjr.tw/15647)
 * [Linux iSCSI Target ACL](http://benjr.tw/15652)
