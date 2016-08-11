#!/usr/bin/env python
# coding=utf-8 ##
import socket, time
import hashlib, os
import sys
username = 'username'  #Define username
password = 'password'      #Define password
server = '202.193.160.123'
print('IP Client for GXNU  (xzpmail@gmail.com)')
print('...........................................')
print('Please input username and password:')
addr = (server, 5300)
liveaddr = (server, 5301)
livekey = 0
sl = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
temple = bytearray([0x82, 0x23, 0x21, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00,
                    0x00, 0x39, 0x67, 0x64, 0x74, 0x34, 0x33, 0x37,
                    0x34, 0x35, 0x77, 0x72, 0x77, 0x71, 0x72, 0x1e,
                    0x00, 0x00, 0x00, 0xFF, 0x74, 0x34, 0x33, 0x37,
                    0x35, 0x42, 0x38, 0x32, 0x35, 0x37, 0x44, 0x44,
                    0x31, 0x35, 0x30, 0x45, 0xFF, 0x44, 0x37, 0x36,
                    0x44, 0x31, 0x35, 0x46, 0x33, 0x35, 0x46, 0x30,
                    0x44, 0x11, 0x00, 0x00, 0x00, 0x31, 0x31, 0x3a,
                    0x32, 0x32, 0x3a, 0x33, 0x33, 0x3a, 0x34, 0x34,
                    0x3a, 0x35, 0x35, 0x3a, 0x36, 0x36, 0x2d, 0x1f,
                    0xd6, 0x03, 0xcc, 0xf2, 0x24, 0x00, 0x0a, 0x00,
                    0x00, 0x00, 0x71, 0x77, 0x65, 0x72, 0x74, 0x79,
                    0x75, 0x69, 0x6f, 0x70])
livepack = bytearray(500)
def send_handshake():
    print('Sending Handshake Packet...')
    global livekey
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.connect(addr)
    pack_send = bytearray(300)
    pack_send[0] = 0x82;
    pack_send[1] = 0x23;
    pack_send[2] = 0x1f
    pack_send[11] = len(username)
    userlen = len(username)
    # print(username)
    tail = bytearray(
        [0x0b, 0x00, 0x00, 0x00, 0x21, 0x40, 0x23, 0x24, 0x25, 0x25, 0x5e, 0x26, 0x2a, 0x28, 0x29, 0x07, 0x00, 0x00,
         0x00, 0x71, 0x77, 0x65, 0x72, 0x74, 0x79, 0x75, 0x39, 0x30, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x00,
         0x00, 0x00, 0x41, 0x53, 0x44, 0x46, 0x47, 0x48])
    for i in range(0, userlen):
        pack_send[15 + i] = ord(username[i]) - 10;
    for i in range(0, 44):
        pack_send[15 + userlen + i] = tail[i];
    try:
        s.send(pack_send)
        datarec = s.recv(1024)
    except socket.error:
        print('Network error')
        quit()
    datarec = bytearray(datarec)
    for i in range(0, 300 - 3):
        if datarec[i] == 0x82 and datarec[i + 1] == 0x23 and datarec[i + 2] == 0x20:
            print('Receiving key packet...')
            calckey = datarec[i + 51 + userlen - 10] + (datarec[i + 52 + userlen - 10] << 8)
            # print ('Key:%x' %calckey)
            calckey = calckey - 3344;
            livekey = calckey + 1500;
            livekey=((livekey>>8) & 0x00ff)+((livekey<<8) & 0xff00);
            src = str(calckey) + password
            # print(src)
            src = src.encode('utf-8')
            md51 = hashlib.md5(src).hexdigest().upper()
            md52 = md51[0:5] + username
            md52 = md52.encode('utf-8')
            md53 = hashlib.md5(md52).hexdigest().upper()
            md53 = md53[0:30]
            md53 = md53.encode('utf-8')
            md53 = bytearray(md53)
            pass_pack = bytearray(300)
            for i in range(0, len(temple)): pass_pack[i] = temple[i]
            for i in range(0, 30): pass_pack[i + 33] = md53[i]
            s.send(pass_pack)
            print('Sending user information...')
            datarec = s.recv(1024)
            break
    datarec = bytearray(datarec)
    for i in range(0, 300):
        if datarec[i] == 0x82 and datarec[i + 1] == 0x23 and datarec[i + 2] == 0x22:
            print('Receiving auth information...')
            if datarec[i + 3] == 0x00:
                print('Connected Succeed...')
                buildlive()
                sendlive()
            elif datarec[i + 3] == 0x63:
                print('Wrong username or password...')
            elif datarec[i + 3] == 0x20:
                print('Account is be occupied...')
            elif datarec[i + 3] == 0x14:
                print('No money...')


def buildlive():  #Construct live heart packet
    global livekey
    mid = bytearray([0xe4, 0x3e, 0x86, 0x02,
                     0x00, 0x00, 0x00, 0x00,
                     0x5c, 0x8f, 0xc2, 0xf5,
                     0xf0, 0xa9, 0xdf, 0x40])
    livepack[0] = 0x82;
    livepack[1] = 0x23;
    livepack[2] = 0x1e;
    livepack[3] = (livekey & 0xff00) >> 8;
    #  print((livekey&0xff00)>>8)
    livepack[4] = (livekey & 0x00ff)
    for i in range(0, 15): livepack[i + 11] = mid[i]
    pos = 0
    for i in range(0, len(username)):
        livepack[31 + i] = ord(username[i])
        pos = 31 + i
    spider = bytearray([0x09, 0x00, 0x00, 0x00, 0x53, 0x70, 0x69, 0x64, 0x65, 0x72, 0x6d, 0x61, 0x6e])
    for i in range(0, 13): livepack[pos + i + 1] = spider[i]


def sendlive():  #send live heart packet every 60 seconds
    while True:
        time.sleep(60)
        sl.sendto(livepack, liveaddr)
       # print "Send Heart Packet..."


# print('Live')

if __name__ == "__main__":
    username=raw_input("Username:")
    password = raw_input('Password: ')
    send_handshake()

