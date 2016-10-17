#使用说明

##前言

最近桂电启用了多运营商宽带出校系统，许多童鞋找我，我看了下，原理一样，所以改了参数，发布了桂电版的路由器MAC开放工具。首先插网线，使用校园网，然后输入路由器的MAC地址，点击预拨号就行，然后路由器该怎么设置就怎么设置。有条件的可以使用实验室不断电电脑开启循环模式，宿舍的路由器就不会掉线了（运营商会在一定时间后自动掉线），如果熟悉linux/unix/os x的童鞋，可以使用python版本

##Windows版本

在校园网环境下（非宽带，建议用插网线，断无线）在路由器MAC地址栏输入路由器的MAC地址即可，然后路由器按照说明书设置pppoe拨号。如果有长期开启的电脑（比如实验室），可以使用循环模式，即使运营商断开后路由器也能自动重连。

##Python版本

路由器预先拨号工具 macopen.py

需要安装numpy包，ubuntu(debian)下执行sudo apt-get install python-numpy python3-numpy

如果你清楚Python版本的话，只需要安装相应版本的numpy即可，不清楚的话建议全部安装

Step1:

修改macopen.py的mac和ip两个变量,mac为需要上网的设备的mac地址，ip为当前机器的ip地址

mac="41:61:86:87:9F:F1"

ip="172.16.54.205" 

Step2:

chmod +x macopen.py

./macopen.py

## MD5校验和

/macopen_guet/macopen_guet_win32.7z ...... f6aeb038421a6ffb91372728aba452a8

/macopen_guet/macopen_guet_win32.exe ..... 111c74de9f9f0616b3969bdf2f997d05

/macopen_guet/macopen_guet_osx.zip ...... 3b9b538a604ad4a20ad2c1893759b91a





