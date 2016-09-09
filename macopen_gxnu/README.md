#MAC开放工具使用说明

##Windows版本

在校园网环境下（非宽带，建议用插网线，断无线）在路由器MAC地址栏输入路由器的MAC地址即可，然后路由器按照说明书设置pppoe拨号。如果有长期开启的电脑（比如实验室里），可以使用循环模式，即使运营商断开后路由器也能自动重连。

##Python版本

路由器预拨号工具macopen.py

Step1:

修改macopen.py的mac和ip两个变量,mac为需要上网的设备的mac地址，ip为当前机器的ip地址

mac="41:61:86:87:9F:F1"

ip="172.16.54.205" 

Step2:

chmod +x macopen.py

./macopen.py