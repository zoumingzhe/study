# GitHub仓库clone失败的解决方法
中国大陆的GitHub服务很不稳定，导致GitHub仓库clone时经常fatal或者hang，下面介绍几种解决方法。

# 解决方案1：ZIP下载
通过GitHub提供的“Download ZIP”方式下载压缩包文件。
## 优点
 - 可以通过迅雷等下载工具实现断点续传。
## 缺点
 - 依赖当前网络对GitHub的访问速度，没有任何的提速。
 - 需要安装迅雷等下载工具，直接通过浏览器下载失败率较高。
 - ZIP包只是某一分支当前的完全拷贝，无法通过git切换commit和分支，更无法fetch后续commit。
## 总结
此种方式只适合获取特定分支当前拷贝，不适合git管理。

# 解决方案2：[gitee码云](https://gitee.com/)
通过[gitee码云](https://gitee.com/)提供的“从GitHub/GitLab导入仓库”功能将GitHub仓库导入gitee后再clone。
## 优点
 - 可以clone并且可以同步GitHub的修改，稳定、快速。
 - clone完成后可以通过gitee在中国大陆稳定访问代码。
## 缺点
 - 需要注册和登陆gitee账号才能clone。
 - clone前无法通过gitee预览仓库，只有clone后才可以通过gitee浏览仓库。
 - gitee对于总仓库、单仓库、单文件的大小有严格的限制，clone时经常会遇到仓库大小超过限制的情况。
## 总结
此种方式适合clone大量的小型仓库。

# 解决方案3：GitHub镜像源
通过将clone的url为镜像源即可通过镜像源clone、push等，但是不能登录访问私有仓库。
目前，可用的GitHub镜像源：
 - https://gitee.com/mirrors
 - https://github.com.cnpmjs.org/
 - https://gitclone.com/
 - https://git.sdut.me/
 - https://hub.fastgit.org/
 - https://api.git.sdut.me/
## 优点
 - 此类镜像源众多，可以随时更换镜像源
 - 只需修改clone的url为镜像源，无需其他操作。
 - 可以直接通过镜像源的url浏览GitHub仓库内容。
 - 一般情况下，可以明显提升clone和fetch的速度。
## 缺点
 - 需要自行寻找可用的镜像源地址并修改clone的url。
 - 镜像网站的限流测量可能导致clone失败，服务不稳定。
 - 对于私有仓库需要进行密码验证，可能无法访问或存在安全风险。
 - 部分镜像网站的镜像时定时更新，clone的可能不是最新的内容，存在延迟。
 - 部分镜像网站只镜像了热门GitHub仓库（Star数量），无法clone冷门仓库。
## 总结
此种方式适合临时clone公共仓库。

# 解决方案4：depth分割
对于大型仓库其clone和fetch数据量较大，但是每个commit的数据量一般较小。clone和fetch是一次原子操作，一旦出错只能从头开始，但是clone和fetch都有depth选项。可以通过depth来对任务进行分割，只要每次depth比上次增加，就可以源源不断的把内容fetch到本地，并且执行出错时不丢失已经fetch完成的内容。
## 优点
 - 没有gitee对于单仓库、单文件的大小的限制。
## 缺点
 - commit较多时操作繁琐，需要不断的修改depth并fetch。
 - 严重依赖当前网络对GitHub的访问速度，没有任何的提速。
 - 当网络极差（一个commit都无法checkout）时，此方法无效。
## 总结
此种方式适合clone个别的大型仓库。

# 解决方案5：VPN
通过VPN访问GitHub。
## 优点
 - 除了需要连接VPN，无需其他改造。
 - 访问相对镜像源方式来说更加稳定。
## 缺点
 - 免费的VPN可能访问不太稳定。
 - 付费的VPN除了要钱没有毛病。
## 总结
此种方式适合有VPN的情况。

# 参考
1.[GitHub仓库快速导入Gitee及同步更新](https://gitee.com/help/articles/4284)

2.[断点续传式git clone（伪）](https://blog.csdn.net/zerooffdate/article/details/79348925)
