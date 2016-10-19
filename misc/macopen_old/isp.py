#!/usr/bin/python
import socket,time,fcntl, struct
import hashlib,os,numpy
import ctypes
bite = ctypes.CDLL('bite.so')
server='202.193.160.123'
addr=(server,20015)
def send_handshake():
 localInfo=bytearray([0x00,0x00,0x00,0x00,0x00,0x00,
                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                         0xac,0x10,0x40,0x12,0x30,0x30,0x3a,0x31,
                         0x46,0x3a,0x31,0x36,0x3a,0x32,0x32,0x3a,
                         0x42,0x38,0x3a,0x45,0x43,0x00,0x00,0x00,
                         0x01,0x00,0x00,0x00,0x00,0x00])
 s1=socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
 s1.connect(addr)
 myMac="00:FF:FF:FF:FF:FF";
 ispKey=0x4e67c6a7;
 localInfo[0]='1'
 nmac=len(myMac)
 nInfo=len(localInfo)
 aaaa=get_local_ip('eth0')
 ipaddress=[0,0,0,0]
 fff=aaaa.split('.')
 for k in range(0,4):
   ipaddress[k]=int(fff[k])
 print(ipaddress)
 for i in range(0,4):
  localInfo[i+30]=ipaddress[i]
 print(nInfo)
 for i in range(0,nmac):
  localInfo[i+34]=myMac[i]
#----------------
 ESI=long(0)
 EBX=long(0)
 ECX=long(0)
 ECX=ispKey
 for i in range(0,nInfo-4):
    ESI=ECX
    ESI=bite.left(ECX, 5)
    # print(">>>>%x"%(ESI))
    if (ECX>0):
      EBX=ECX
      EBX=bite.righ(ECX, 2)
    else:
      EBX=ECX;
      EBX=bite.righ(ECX, 2)
      EBX=EBX|(0xC0000000)
    ESI=ESI+int(localInfo[i])
    EBX=bite.plus(EBX,ESI)
    ECX=ECX^EBX
# print("%x"%(ECX))
 ECX=ECX&(0x7FFFFFFF)
#----------------
#print(localInfo)
#print("%x"%(ECX))
 for i in range(0,4):
  keypart=(bite.righ(ECX,i*8))&0x000000FF;
  localInfo[nInfo-(4-i)]=keypart
 s1.send(localInfo)

def get_local_ip(ifname):
 s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
 inet = fcntl.ioctl(s.fileno(), 0x8915, struct.pack('256s', ifname[:15]))
 ret = socket.inet_ntoa(inet[20:24])
 return ret

if __name__=="__main__":
    send_handshake()
    #    print get_local_ip("eth0")
    exit
