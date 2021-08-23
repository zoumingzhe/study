Targetcli Discovery Control
===========================
iSCSI协议可以控制iSCSI Target对iSCSI Initiator Discovery的可见性。

# CHAP Initiator Discovery Authentication
通过在全局discovery_auth组中设置CHAP信息userid（login name）和password（target secret），为所有initiator启用单向CHAP探索身份验证：

```shell
/iscsi> set discovery_auth enable=1 userid=iqn.2003-01.org.linux-iscsi.demo:auth password=password_demo
Parameter enable is now 'True'.
Parameter password is now 'password_demo'.
Parameter userid is now 'iqn.2003-01.org.linux-iscsi.demo:auth'.
/iscsi> get discovery_auth
DISCOVERY_AUTH CONFIG GROUP
===========================
enable=True
-----------
The enable discovery_auth parameter.

mutual_password=
----------------
The mutual_password discovery_auth parameter.

mutual_userid=
--------------
The mutual_userid discovery_auth parameter.

password=password_demo
----------------------
The password discovery_auth parameter.

userid=iqn.2003-01.org.linux-iscsi.demo:auth
--------------------------------------------
The userid discovery_auth parameter.
```

现在，只有使用userid=iqn.2003-01.org.linux-iscsi.demo:auth和password=password_demo进行身份验证的iSCSI Initiator才能探索此iSCSI Target。

# Mutual CHAP Discovery Authentication
通过在全局discovery_auth组中设置Mutual CHAP信息，为所有initiator启用双向CHAP探索身份验证：
 - target的userid（login name）和password（target secret）。
 - initiator的mutual_userid（login name）和mutual_password（initiator secret）。

```shell
/iscsi> set discovery_auth enable=1 userid=iqn.2003-01.org.linux-iscsi.demo:auth password=password_demo mutual_userid=iqn.2003-01.org.linux-iscsi.demo:auth_mutual mutual_password=password_demo_mutual
Parameter userid is now 'iqn.2003-01.org.linux-iscsi.demo:auth'.
Parameter password is now 'password_demo'.
Parameter mutual_password is now 'password_demo_mutual'.
Parameter mutual_userid is now 'iqn.2003-01.org.linux-iscsi.demo:auth_mutual'.
Parameter enable is now 'True'.
/iscsi> get discovery_auth
DISCOVERY_AUTH CONFIG GROUP
===========================
enable=True
-----------
The enable discovery_auth parameter.

mutual_password=password_demo_mutual
------------------------------------
The mutual_password discovery_auth parameter.

mutual_userid=iqn.2003-01.org.linux-iscsi.demo:auth_mutual
----------------------------------------------------------
The mutual_userid discovery_auth parameter.

password=password_demo
----------------------
The password discovery_auth parameter.

userid=iqn.2003-01.org.linux-iscsi.demo:auth
--------------------------------------------
The userid discovery_auth parameter.
```

现在，只有使用userid=iqn.2003-01.org.linux-iscsi.demo:auth和password=password_demo进行身份验证的iSCSI Initiator才能探索此iSCSI Target。

并且，iSCSI Target只能探索使用userid=iqn.2003-01.org.linux-iscsi.demo:auth_mutual和password=password_demo_mutual进行双向身份验证的iSCSI Initiator。

# 参考
 * [LIO Admin Manual](http://www.linux-iscsi.org/Doc/LIO%20Admin%20Manual.pdf)
