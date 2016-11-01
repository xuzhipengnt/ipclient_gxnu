#!/usr/bin/env python
# coding=utf-8 ##
################This is MAC OPEN TOOLS  Version 1.0###################
################Only for GUET###########################
import socket
server='172.16.1.1'  #GUET 172.16.1.1  GXNU 202.193.160.123
addr=(server,20015)

def int_overflow(val):
    maxint = 2147483647
    if not -maxint-1 <= val <= maxint:
        val = (val + (maxint + 1)) % (2 * (maxint + 1)) - maxint - 1
    return val

def send_handshake(account,mac,ip,isp):
 localInfo=bytearray([0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                         0x00,0x00,0x00,0x00,0x00,0x00,0x0a,0x14,
                         0x29,0x1e,0x44,0x38,0x3a,0x43,0x42,0x3a,
                         0x38,0x41,0x3a,0x38,0x34,0x3a,0x44,0x46,
                         0x3a,0x31,0x39,0x00,0x00,0x00,0x01,0x00,
                         0x00,0x00,0x00,0x00])
 s1=socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
 s1.connect(addr)
 ispKey=0x4e67c6a7
 localInfo[0]=0x61
 nmac=len(mac)
 nInfo=len(localInfo)
 nAccount=len(account)
 for i in range(0,nAccount):
  localInfo[i]=ord(account[i])
  
 ipaddress=[0,0,0,0]
 fff=ip.split('.')
 for k in range(0,4):
   ipaddress[k]=int(fff[k])
 #print(ipaddress)
 for i in range(0,4):
  localInfo[i+30]=ipaddress[i]
 #print(nInfo)
 for i in range(0,nmac):
  localInfo[i+34]=ord(mac[i])
 localInfo[54]=isp
#----------------
 ESI=int(0)
 EBX=int(0)
 ECX=int(0)
 ESI2=int(0)
 ECX=int(ispKey)
 for i in range(0,nInfo-4):
    ESI=ECX
    ESI=int_overflow(ECX<<5)
    if (ECX>0):
      EBX=ECX
      EBX=ECX>>2
    else:
      EBX=ECX
      EBX=ECX>>2
      EBX=EBX|(0xC0000000)
    ESI=ESI+int(localInfo[i])
    EBX=int_overflow(EBX+ESI)
    ECX=ECX^EBX
 ECX=ECX&(0x7FFFFFFF)

 for i in range(0,4):
  keypart=((ECX>>(i*8))&0x000000FF)
  localInfo[nInfo-(4-i)]=keypart
 s1.send(localInfo)
     #for i in range(0,nInfo):
#print("%x"%localInfo[i])
 
def get_local_ip(ifname):
 s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
 inet = fcntl.ioctl(s.fileno(), 0x8915, struct.pack('256s', ifname[:15]))
 ret = socket.inet_ntoa(inet[20:24])
 return ret

if __name__=="__main__":
    account="11" ##User account
    mac="11:11:11:11:11:11"  ##Mac Address
    ip="11.11.11.11" ##!!!!ip is local machine's ip address but not router's ip
    isp=0x01
    ###isp  0x01(China Unicom)  0x02(China Telecom)  0x03(China Mobile) ###
    send_handshake(account,mac,ip,isp)
    send_handshake(account,mac,ip,isp)
    print("Finished...")
    exit

