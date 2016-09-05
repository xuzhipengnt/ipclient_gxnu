#!/usr/bin/env python
# coding=utf-8 ##
import socket,numpy
server='172.16.1.1'  #GUET 172.16.1.1  GXNU 202.193.160.123
addr=(server,20015)
def send_handshake(mac,ip,isp):
 localInfo=bytearray([0x00,0x00,0x00,0x00,0x00,0x00,
                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                         0xac,0x10,0x40,0x12,0x30,0x30,0x3a,0x31,
                         0x46,0x3a,0x31,0x36,0x3a,0x32,0x32,0x3a,
                         0x42,0x38,0x3a,0x45,0x43,0x00,0x00,0x00,
                         0x03,0x00,0x00,0x00,0x00,0x00])
 s1=socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
 s1.connect(addr)
 ispKey=0x4e67c6a7
 localInfo[0]=0x61
 nmac=len(mac)
 nInfo=len(localInfo)
 ipaddress=[0,0,0,0]
 fff=ip.split('.')
 for k in range(0,4):
   ipaddress[k]=int(fff[k])
 print(ipaddress)
 for i in range(0,4):
  localInfo[i+30]=ipaddress[i]
 print(nInfo)
 for i in range(0,nmac):
  localInfo[i+34]=mac[i]
  localInfo[54]=isp
#----------------
 ESI=int(0)
 EBX=int(0)
 ECX=int(0)
 ESI2=int(0)
 ECX=int(ispKey)
 for i in range(0,nInfo-4):
    ESI=ECX
    ESI=numpy.int32(ECX<<5)
    if (ECX>0):
      EBX=ECX
      EBX=ECX>>2
    else:
      EBX=ECX
      EBX=ECX>>2
      EBX=EBX|(0xC0000000)
    ESI=ESI+int(localInfo[i])
    EBX=numpy.int32(EBX+ESI)
    ECX=ECX^EBX
 ECX=ECX&(0x7FFFFFFF)

 for i in range(0,4):
  keypart=((ECX>>(i*8))&0x000000FF)
  localInfo[nInfo-(4-i)]=keypart
 s1.send(localInfo)

def get_local_ip(ifname):
 s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
 inet = fcntl.ioctl(s.fileno(), 0x8915, struct.pack('256s', ifname[:15]))
 ret = socket.inet_ntoa(inet[20:24])
 return ret

if __name__=="__main__":
    mac="40:61:86:87:9F:F1"
    ip="172.16.1.1" ##!!!!in GXNU, ip is local machine's ip address but not router's ip
                    ##!!!!in GUET, do not modify ip
    isp=0x01
    ###isp  0x01(China Unicom)  0x02(China Telecom)  0x03(China Mobile) ###
    send_handshake(mac,ip,isp)
    exit

