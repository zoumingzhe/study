Targetcli Access Control
========================
LUN访问身份验证有三种基本设置：演示模式（demo mode）、单向CHAP身份验证、双向CHAP身份验证。

# demo mode
“演示模式”描述了一种安全配置，其禁用了对TPG中LUN的所有访问控制，在TPG中使用自动ACL而不进行身份验证。

这种配置被称为“演示模式”，因为它通常只适用于简单的演示设置。

启用“演示模式”：
```shell
/iscsi/<IQN>/<TPG>> set attribute authentication=0 demo_mode_write_protect=0 generate_node_acls=1 cache_dynamic_acls=1
Parameter demo_mode_write_protect is now '0'.
Parameter authentication is now '0'.
Parameter generate_node_acls is now '1'.
Parameter cache_dynamic_acls is now '1'.
```

[Datera](https://datera.io/)强烈建议仅在以下情况下使用“演示模式”：
 - 已经通过一个封闭、受控的SAN环境建立了物理安全。
 - 将SAN与集群文件系统结合使用，集群文件系统通常通过分布式锁定来保证多个initiator之间的一致性。
 - 已经仔细分析了ACL设置的安全性和数据完整性要求以及风险。

# CHAP Initiator Authentication
设置initiator的单向CHAP登录信息，该信息包括来自initiator的userid（login name）和password（target secret）：

```shell
/iscsi/<IQN>/<TPG>/acls/<IQN>> set auth userid=iqn.2003-01.org.linux-iscsi.demo:auth
Parameter userid is now 'iqn.2003-01.org.linux-iscsi.demo:auth'.
/iscsi/<IQN>/<TPG>/acls/<IQN>> set auth password=password_demo
Parameter password is now 'password_demo'.
/iscsi/<IQN>/<TPG>/acls/<IQN>> get auth
AUTH CONFIG GROUP
=================
mutual_password=
----------------
The mutual_password auth parameter.

mutual_userid=
--------------
The mutual_userid auth parameter.

password=password_demo
----------------------
The password auth parameter.

userid=iqn.2003-01.org.linux-iscsi.demo:auth
--------------------------------------------
The userid auth parameter.
```

# Mutual CHAP Authentication
设置initiator的双向CHAP登录信息，包括：
 - target的userid（login name）和password（target secret）。
 - initiator的mutual_userid（login name）和mutual_password（initiator secret）。

```shell
/iscsi/<IQN>/<TPG>/acls/<IQN>> set auth userid=iqn.2003-01.org.linux-iscsi.demo:auth
Parameter userid is now 'iqn.2003-01.org.linux-iscsi.demo:auth'.
/iscsi/<IQN>/<TPG>/acls/<IQN>> set auth password=password_demo
Parameter password is now 'password_demo'.
/iscsi/<IQN>/<TPG>/acls/<IQN>> set auth mutual_userid=iqn.2003-01.org.linux-iscsi.demo:auth_mutual
Parameter mutual_userid is now 'iqn.2003-01.org.linux-iscsi.demo:auth_mutual'.
/iscsi/<IQN>/<TPG>/acls/<IQN>> set auth mutual_password=password_demo_mutual
Parameter mutual_password is now 'password_demo_mutual'.
/iscsi/<IQN>/<TPG>/acls/<IQN>> get auth
AUTH CONFIG GROUP
=================
mutual_password=password_demo_mutual
------------------------------------
The mutual_password auth parameter.

mutual_userid=iqn.2003-01.org.linux-iscsi.demo:auth_mutual
----------------------------------------------------------
The mutual_userid auth parameter.

password=password_demo
----------------------
The password auth parameter.

userid=iqn.2003-01.org.linux-iscsi.demo:auth
--------------------------------------------
The userid auth parameter.
```

# TPG Authentication
单独为每个initiator设置身份验证信息可能会很麻烦，因此targetcli提供了为TPG中的所有端点定义通用身份验证信息的功能。

因此，连接到该TPG的所有initiator都可以使用相同的身份验证信息。

首先，必须为TPG中的所有端点启用公共TPG身份验证：

```shell
/iscsi/<IQN>/<TPG>> set attribute authentication=1 demo_mode_write_protect=0 generate_node_acls=1 cache_dynamic_acls=1
Parameter demo_mode_write_protect is now '0'.
Parameter authentication is now '1'.
Parameter generate_node_acls is now '1'.
Parameter cache_dynamic_acls is now '1'.
```

然后，为TPG中的所有端点设置公共TPG身份验证，包括：
 - target的userid（login name）和password（target secret）。
 - initiator的mutual_userid（login name）和mutual_password（initiator secret）。

```shell
/iscsi/<IQN>/<TPG>/acls/<IQN>> set auth userid=iqn.2003-01.org.linux-iscsi.demo:auth
Parameter userid is now 'iqn.2003-01.org.linux-iscsi.demo:auth'.
/iscsi/<IQN>/<TPG>/acls/<IQN>> set auth password=password_demo
Parameter password is now 'password_demo'.
/iscsi/<IQN>/<TPG>/acls/<IQN>> set auth mutual_userid=iqn.2003-01.org.linux-iscsi.demo:auth_mutual
Parameter mutual_userid is now 'iqn.2003-01.org.linux-iscsi.demo:auth_mutual'.
/iscsi/<IQN>/<TPG>/acls/<IQN>> set auth mutual_password=password_demo_mutual
Parameter mutual_password is now 'password_demo_mutual'.
```

*可以通过添加相应的ACL条目来创建特定initiator的登录凭据，因为单个ACL条目覆盖公共TPG身份验证信息。*

# 参考
 * [LIO Admin Manual](http://www.linux-iscsi.org/Doc/LIO%20Admin%20Manual.pdf)
