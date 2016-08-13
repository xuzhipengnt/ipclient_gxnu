#coding:utf-8
import hashlib
import string
import os
stq=['1','2','3','4','5','6','7','8','9','0']
username='username'
key1=0x246f
hash1='51A057BEF3DE691F78E2B21C8D8044'
key2=0x1195
hash2='436CC17B8D83FF3AF055BD46675819'
for key in range(0,9999999):
                src = str(key1-3344)+str(key)
                md51=hashlib.md5(src).hexdigest().upper()
                md52=md51[0:5]+username
                md53=hashlib.md5(md52).hexdigest().upper()
                if md53[0:30]==hash1:
                    print src
                    print md51
                    print md52