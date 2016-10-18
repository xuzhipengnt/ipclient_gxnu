#!/usr/bin/env python
# coding=utf-8 ##
################This is MAC OPEN TOOLS###################
################Special Verion Scapy#################
################Only for GXNU###########################
from scapy import *
from scapy.all import *
def int_overflow(val):
    maxint = 2147483647
    if not -maxint-1 <= val <= maxint:
        val = (val + (maxint + 1)) % (2 * (maxint + 1)) - maxint - 1
    return val

def send_handshake(mac,isp):
 localInfo=bytearray([0x00,0x00,0x00,0x00,0x00,0x00,
                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                         0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                         0xac,0x10,0x40,0x12,0x30,0x30,0x3a,0x31,
                         0x46,0x3a,0x31,0x36,0x3a,0x32,0x32,0x3a,
                         0x42,0x38,0x3a,0x45,0x43,0x00,0x00,0x00,
                         0x03,0x00,0x00,0x00,0x00,0x00])
 ispKey=0x4e67c6a7
 localInfo[0]=ord('1')
 nmac=len(mac)
 nInfo=len(localInfo)
 ipaddress=[172,19,92,11]
 for i in range(0,4):
  localInfo[i+30]=ipaddress[i]
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
 fakepack=struct.pack('60B',*localInfo)
 pkt2=IP(src="172.19.92.11", dst='202.193.160.123')/UDP(sport=12345,dport=20015)/fakepack
 send(pkt2)

if __name__=="__main__":
    mac="FF:FF:FF:87:9F:F1"
    isp=0x01
    ###isp  0x01(China Unicom)  0x02(China Telecom)  0x03(China Mobile) ###
    send_handshake(mac,isp)
    exit

