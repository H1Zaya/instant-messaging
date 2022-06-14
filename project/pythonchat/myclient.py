from encodings import utf_8
from reprlib import aRepr
from socket import *
import threading
import time
from tkinter import N

outString = ''
inString = ''
names = ''
str1 = '    '

ttday = time.strftime("%Y.%m.%d %a", time.localtime())
tday = time.strftime("%H:%M", time.localtime())


def csend(sock):
    global outString
    while True:
        outString = input()
        outString = tday + ' '+ names + ':'+ str1 + outString
        sock.send(outString.encode('utf-8'))


def caccept(sock):
    global inString
    while True:
        try:
            inString = sock.recv(1024).decode().strip()
            if not inString:
                break
            if outString != inString:
                print(inString)
        except:
            break

ip = input('input your destination ip address: ')
names = input('input your name: ')


sock = socket(AF_INET,SOCK_STREAM)
sock.connect((ip, 8888))
sock.send(names.encode('utf-8'))  # username send to server

th_send = threading.Thread(target=csend, args=(sock,))
th_send.start()
th_accept = threading.Thread(target=caccept, args=(sock,))
th_accept.start()
