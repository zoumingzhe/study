iscsid CHAP Settings
====================

Linux主机`/etc/iscsi/initiatorname.iscsi`配置的InitiatorName。

Linux主机`/etc/iscsi/iscsid.conf`或`/etc/iscsid.conf`配置的CHAP段落：
```shell
# *************
# CHAP Settings
# *************

# To enable CHAP authentication set node.session.auth.authmethod
# to CHAP. The default is None.
#node.session.auth.authmethod = CHAP

# To set a CHAP username and password for initiator
# authentication by the target(s), uncomment the following lines:
#node.session.auth.username = username
#node.session.auth.password = password

# To set a CHAP username and password for target(s)
# authentication by the initiator, uncomment the following lines:
#node.session.auth.username_in = username_in
#node.session.auth.password_in = password_in

# To enable CHAP authentication for a discovery session to the target
# set discovery.sendtargets.auth.authmethod to CHAP. The default is None.
#discovery.sendtargets.auth.authmethod = CHAP

# To set a discovery session CHAP username and password for the initiator
# authentication by the target(s), uncomment the following lines:
#discovery.sendtargets.auth.username = username
#discovery.sendtargets.auth.password = password

# To set a discovery session CHAP username and password for target(s)
# authentication by the initiator, uncomment the following lines:
#discovery.sendtargets.auth.username_in = username_in
#discovery.sendtargets.auth.password_in = password_in
```

**注意：**
 - *修改iscsid配置必须执行`systemctl restart iscsid`重启iscsid使配置生效。*
 - *修改CHAP配置信息只有重新探索target后session的auth信息才会更新。*

# 探索
`iscsiadm --mode discoverydb --type sendtargets --portal <ip> --discover`

探索需要身份验证的target时，需修改CHAP设置中的探索验证信息：
```shell
discovery.sendtargets.auth.authmethod = CHAP
discovery.sendtargets.auth.username = <initiator's user name>
discovery.sendtargets.auth.password = <CHAP secret for host>
```
若探索需要双向身份验证时，需修改CHAP设置中的探索双向验证信息：
```shell
discovery.sendtargets.auth.username_in = <target's user name>
discovery.sendtargets.auth.password_in = <CHAP secret for clustered system>
```

# 登入
登入所有探索的target：
`iscsiadm -m node -L all`

登入指定target：
`iscsiadm -m node -T <iqn> -l`

当登入需要身份验证的target且需修改对应session的验证信息时：
 - 开启验证
`iscsiadm -m node -T <iqn> -o update -n node.session.auth.authmethod -v=CHAP`
 - 更新单向验证用户
`iscsiadm -m node -T <iqn> -o update -n node.session.auth.username -v=<username>`
 - 更新单向验证密码
`iscsiadm -m node -T <iqn> -o update -n node.session.auth.password -v=<password>`
 - 更新双向验证信息
`iscsiadm -m node -T <iqn> -o update -n node.session.auth.username_in -v=<username>`
 - 更新双向验证密码
`iscsiadm -m node -T <iqn> -o update -n node.session.auth.password_in -v=<password>`

# 登出
登出所有登入的target：
`iscsiadm -m node -U all`

登出指定target：
`iscsiadm -m node -T <iqn> -u`

# 查看session
`iscsiadm -m session [-P <0-3>]`

# 启动自动登入
`iscsiadm -m node -o update -n node.startup -v automatic`

*可与-T选项结合使用，automatic为自动，manual为手动。*

# 参考
 * [設定 Linux 主機的鑑別](https://www.ibm.com/docs/zh-tw/spectrumvirtualsoftw/8.1.x?topic=initiator-setting-up-authentication-linux-hosts)
