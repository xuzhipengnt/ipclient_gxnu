# 广西师大的出校控制工具开源实现，另更新桂林电子科大的路由器辅助拨号工具（多运营商出校控制工具）

## 前言 Preface

本人即将于明年毕业，研究生三年基本上属于“不务正业”的状态。

今天用某杀毒软件居然扫出广西师大出校控制器有毒，于是“怒”决定把研究过的出校控制器的协议进行开源，造福各位师弟师妹。
不仅包括校园网的协议，还有的宽带的协议。

学校的校园网协议大概说个框架，具体还需要看/doc下的文档。就是客户端向服务器发送用户名，服务器返回一个salt，客户端再根据用户名、密码和salt计算出摘要返回给服务器，密码正确就修改ACL(也有可能是firewall)，让数据包通过，就能正常上网。为了防止不正常退出，还在计费引起损失，所以引入心跳包，长期收不到心跳包就自动断开。因为有心跳包的缘故，而且师大只要输错密码，机器就会掉线，与之前状态无关，因此断开的协议也就没有必要了（师大一个帐号即使在一台机器上断开了，也需要过个十分钟才能在另一台机器上用）

学校的宿舍宽带其实没什么特殊，只是先发一组包含MAC地址信息数据包给服务器202.193.160.123（桂林电子科大服务器地址为172.16.1.1），然后服务器再向交换机下发指令修改ACL(也有可能是firewall)将某MAC地址流量引入联通、移动或者电信的pppoe服务器，这样就可以使得多个运营商pppoe服务器存在于一个网络而不冲突，为什么是MAC地址呢，因为pppoe协议是基于MAC地址，而非ip，所以在家用宽带，即使无法获取ip地址还是可以正常拨号上网的原因。学校的出校控制器是直接用机器网卡的MAC地址，所以如果在路由器上通过MAC地址复制也是可以使用的，但是在局域网内两台相同的MAC地址会引起通信问题。而我们如果也能构造同样的数据包，但是MAC地址自己输入，就可以使得路由器正常拨号了，因此路由器预拨号工具也可以称作MAC开放工具(macopen tools)。

无论是出校控制器还是出校控制还是写出了QT和Python的两个版本，QT的好处是界面好看，Python的好处是代码短，两个都能跨所有平台，Python用在服务器上更方便一点。

企鹅号648049***的童鞋跟我反映，\macopen_gxnu\macopen.py在树莓派下的numpy报错，这里讲一下历史，刚开始做这个工具的时候，发现python对int的溢出将自动转换为long型，于是用c写了个动态链接库，将+-<<>>等基础操作封装起来，然后python去调用这个库，不同操作系统动态链接库是需要重新编译的（见\misc\macopen_old），因此不适合大规模使用，所以后来引入了numpy来处理这个问题，而numpy貌似在树莓派下会出错，所以现在参考别人的资料，不再采用numpy库。最后感谢童鞋提出的bug,在此表示感谢！

还有会有人问为什么要开源呢？

这些协议只是学校的私有的协议，也就桂林这一带的高校才用，逆向了也没什么商业利益，纯粹兴趣和个人需要，所以就开源啦，就是辣么任性，哈哈哈～～

## 联系方式 Contact

如有疑问可以首先参阅/doc，如果没能解决，内心愤怒想吐槽作者的表达能力，想吐槽代码写的太烂或者发现有Bug，甚至想自己动手改进，欢迎联系xzpmail@gmail.com，有空一定及时回复。

## 目录结构 Index

出校控制器命令行版本如下

/binary/ipclient_cli_x64_linux........64位linux的命令行版本

/binary/ipclient_cli_linux........32位linux的命令行版本（Linux用户推荐）

/binary/ipclient_cli_x64_osx........64位osx的命令行版本

出校控制器带界面版本如下

/binary/ipclient_gui_osx.app........OS X下面有界面的出校控制器(OS X用户推荐，需下载macopen_gui_osx.zip路由器拨号工具来使用，卖得起apple，路由器也应该买得起吧，哈哈哈)

/binary/ipclient_gui_win32_7z.7z........Windows下面有界面的出校控制器（需解压后使用）

/binary/ipclient_gui_win32_binary.exe........Windows下面有界面的出校控制器（直接使用,Windows用户推荐）


广西师大路由器辅助拨号工具如下，只需要输入路由器的Mac地址和运营商，路由器就可以正常上网

/macopen_gxnu/macopen_gxnu_osx.zip.......For Mac

/macopen_gxnu/macopen_gxnu_win32.7z........For Windows

/macopen_gxnu/macopen_gxnu_win32.exe.......For Windows 单独Exe

/macopen_gxnu/src.......路由器辅助拨号工具源码

/macopen_gxnu/macopen.py......路由器辅助拨号工具Python版本

文档和源码如下

/doc/原理文档.pdf.......学校出校控制器，宿舍宽带拨号的秘密都在里面啦

/doc/原理文档(Latex源码)......原理文档的Latex源码，编译成pdf

/doc/桂林电子科大学生的逆向文档.........桂电学生做的，也是最原始的资料，师大改了一些参数

/ipclient_c......c版本的出校控制器命令行版本的源码，在别人的基础上改了一下参数而已

/ipclient_qt.......出校控制器界面的qt源码


## 新增桂林电子科大路由器辅助拨号工具  MAC OPEN TOOLS FOR GUET

/macopen_guet/src.......路由器辅助拨号工具qt源码(桂林电子科大)

/macopen_guet/macopen.py......路由器辅助拨号工具Python版本

/macopen_guet/macopen_guet_win32.exe......路由器辅助拨号工具的win32二进制版本

/macopen_guet/macopen_guet_win32.7z......路由器辅助拨号工具的win32二进制压缩版本（需要解压）

## 更新日志 Change log

--------2016.8.11-----------

1、建一个库，放好第一块砖

2、上传./ipclient_python/ipclient.py 校园网出校控制器Python实现，童鞋么可以在Linux,OS X,Windows下上网了，开源的用了更放心～～

--------2016.8.12-----------

1、上传ipclient的c版本源码(osx/linux通用,windows需要装Mingw，没试过)

2、上传ipclient的c版本的osx的编译后二进制包

3、上传ipclient_qt的源码

4、上传ipclient_qt的windows版本的二进制包

5、上传ipclient的linux命令行版本

6、上传原理文档，在/doc/原理文档.pdf里面，这才是最关键的哈哈哈～～

7、上传路由器宽带辅助拨号工具

--------2016.8.13-----------

1、修正路由器宽带辅助拨号工具Bug

2、上传路由器宽带辅助拨号工具For windows

3、上传os x pppoe协议栈的问题，详细分析为什么苹果电脑无法正常通过pppoe拨号上网，见/doc/苹果为什么在师大无法拨号.docx

--------2016.8.16-----------

1、路由器宽带辅助拨号工具增加指示灯，依据返回数据包，黄灯表示开放成功

--------2016.9.5-----------

1、增加桂电路由器拨号工具Python版本 /macopen_guet/macopen.py

2、增加桂电路由器拨号工具Windows二进制版本 /macopen_guet/macopen_guet_win32.exe(7z) 

--------2016.9.7-----------

1、修正了原理文档中的书写错误

2、上传原理文档的Latex源码

--------2016.9.8-----------

1、修复了桂电mac开放工具ip地址可能解析不正确的错误

2、修复了师大mac开放工具ip地址可能解析不正确的错误，并加上标示，以防与桂电混淆

3、修改了部分目录结构 \macopen-->\macoepn_gxnu

4、增加了桂电路由器拨号工具的OS X版本 /macopen_guet/macopen_guet_osx.zip

--------2016.9.10-----------

1、上传EXE打包工具enigma virtual box(这是免费软件，官网http://enigmaprotector.com/en/aboutvb.html)

2、用最新的enigma virtual box重新打包并上传/macopen_guet/macopen_guet_win32.exe,/macopen_gxnu/macopen_gxnu_win32.exe,/binary/ipclient_gui_win32_binary.exe

3、有童鞋反映360报毒的问题，我不用360，但是我这里avast,卡巴斯基均正常。若不放心可下载7z压缩包解压后运行。

4、增加md5校验码，用于检验程序下载过程是否受到篡改

--------2016.9.23-----------

1、增加GPL V3声明

--------2016.10.17-----------

1、修正\macopen_gxnu\macopen.py在Python3下的类型转换问题，修复了小Bug

2、修正\macopen_guet\macopen.py在Python3下的类型转换问题，修复了小Bug

3、在\macopen_guet \macopen_gxnu的README中增加安装numpy的建议

--------2016.10.18-----------

1、去除\macopen_gxnu\macopen.py对numpy库的依赖

2、去除\macopen_guet\macopen.py对numpy库的依赖

##二进制包的MD5校验和  MD5SUM

/binary/ipclient_cli_x64_linux ...... 41c8355b562e2b6c51dc291ddcc7c52b

/binary/ipclient_cli_x64_osx ...... 53ae4e497ba929617a229e02430bc92a

/binary/ipclient_cli_linux ...... e6875f7b5602cd211d1c681abf851618

/binary/ipclient_gui_osx.zip ...... 595c79e2fb1a3d82867e358be8651c88

/binary/ipclient_gui_win32_7z.7z ...... 047f04da46cebff69f36ceb07a779bc8

/binary/ipclient_gui_win32_binary.exe ...... 11ca323a68f5476fa700d72a4bcdfe12

/macopen_gxnu/macopen_gxnu_win32.7z ...... f8563fca042c823c9c1ca35615619c70

/macopen_gxnu/macopen_gxnu_win32.exe ...... 1dd7b2bc0cbf4bbbcf7517e292e32c24

/macopen_gxnu/macopen_gxnu_osx.zip ...... c0b22189f8a0103386362f3f4ef27ced

/macopen_guet/macopen_guet_win32.7z ...... f6aeb038421a6ffb91372728aba452a8

/macopen_guet/macopen_guet_win32.exe ..... 111c74de9f9f0616b3969bdf2f997d05

/macopen_guet/macopen_guet_osx.zip ...... 3b9b538a604ad4a20ad2c1893759b91a

##版权 Copyright

(1)以上软件以及相应文档使用GNU General Public Licence v3 (以下简称GPL) 发布其源代码和二进制代码，任何人都可以在不违反GPL的情况下获取，修改和重新发布代码。但是应该保留原作者和版权声明。http://www.gnu.org/copyleft/gpl.html

（中文参考：http://www.thebigfly.com/gnu/gpl/)

(2)以上软件中涉及的协议归广西师范大学、桂林电子科技大学网络中心所有。

(3)根据GPL，由于以上程序是无偿授权，因此在法律许可范围內，本授权对本程序并不负担保责任。非经法律要求或书面同意，任何著作权人或任何可能依前述方式修改与／或发布本程序者，对于您因为使用或不能使用本程序所造成的一般性、特殊性、意外性或间接性损失，不负任何责任（包括但不限于，资料损失，资料执行不精确，或应由您或第三人承担的损失，或本程序无法与其他程序运作等），即便前述的著作权人或其他人已被告知该等损失的可能性时，也是一样。

