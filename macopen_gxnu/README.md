#MAC开放工具使用说明

##Windows版本

在校园网环境下（非宽带，建议用插网线，断无线）在路由器MAC地址栏输入路由器的MAC地址即可，然后路由器按照说明书设置pppoe拨号。如果有长期开启的电脑（比如实验室里），可以使用循环模式，即使运营商断开后路由器也能自动重连。

##Python版本(macopen.py)

路由器预拨号工具macopen.py

Step1:

修改macopen.py的mac和ip和isp三个变量,mac为需要上网的设备的mac地址，ip为当前机器的ip地址,isp为运营商

mac="41:61:86:87:9F:F1"

ip="172.16.54.205"

isp=0x01 (0x01 联通 0x02 电信 0x03 移动)

Step2:

chmod +x macopen.py

./macopen.py

##scapy特殊Python版本(macopen_scapy.py)

一般python不带scapy模块，因此需要安装scapy

ubuntu(debian)下

sudo apt-get install scapy python3-pip

pip3 install scapy

修改macopen_scapy.py中的mac和isp变量（同上）

chmod +x macopen_scapy.py

sudo ./macopen_scapy.py

## MD5校验和

/macopen_gxnu/macopen_gxnu_win32.7z ...... e131d19624b4cada0bc47ca48911311b

/macopen_gxnu/macopen_gxnu_win32.exe ...... d3c1860d8fd88f846de77848fe50d457

/macopen_gxnu/macopen_gxnu_osx.zip ...... 6f7e3b33cc8090ec597ce3dc5361af41
