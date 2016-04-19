#!/usr/bin/python

import socket
import sys, random
import time
from thread import *
import subprocess

#HOST = ''
HOST = '192.168.1.1'
PORT = 8888

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

VU_CACHE = 120
LED_MODE = 0
LED_IDX = 0

COLORS = ("128 0 0", "0 128 0", "0 0 128","128 0 0","0 128 0","128 128 0","0 128 128", "128 0 128")

print 'Socket created'

try:
    s.bind((HOST, PORT))
except socket.error as msg:
    print 'Bind failed. Error Code : ' + str(msg[0]) + ' Message ' + msg[1]
    sys.exit()

print 'Socket bind complete'

s.listen(10)
print 'Socket now listening'

def changeColor():
    global CUR_FOREGROUND
    global CUR_BACKGROUND
    global COLORS
    rand = random.randrange(0,len(COLORS))
    CUR_FOREGROUND = COLORS[rand]
    CUR_BACKGROUND = COLORS[(rand+1)%len(COLORS)]

def soundthread():
    bashCommand = "./sound.sh"
    process = subprocess.Popen(bashCommand.split(), stdout=subprocess.PIPE)
    output = process.communicate()[0]
    print "TODO"

def clientSend(conn, in_str):
    print "TX> " + in_str
    conn.sendall(in_str)

def clientthread(conn):
    global VU_CACHE
    global LED_MODE
    global LED_IDX
    while True:
  data_prepared = 0
  data = conn.recv(1024)
  if data:
      if int(data) > 160:
    data = str(160)
      data_prepared = (int(data) - 0) * (120 - 1) / (160 - 0) + 1
      print "RX> "+ data + " RECALCULED> " + str(data_prepared)

      if VU_CACHE < data_prepared and data_prepared <= 120 and int(data) > 3:
    VU_CACHE = data_prepared
    if data_prepared > 105:
        LED_MODE = 1
#        start_new_thread(soundthread, ())


  VU_CACHE = VU_CACHE - 1
  if VU_CACHE < 1:
      VU_CACHE = 120

#  LED_MODE = 1

#  if LED_MODE == 3:
#      clientSend(conn,"c 0\n")
#      clientSend(conn,"r 200 200 200 1 120\n")
#      clientSend(conn,"c 1\n")

  if LED_MODE == 0:
      outrnd = "c 0" + " \n"
            clientSend(conn,outrnd)
#      outrnd = "s " + str(VU_CACHE) + " " + str(0) + " " + str(128) + " " + str(0) + " \n"
#      outrnd = "s " + str(VU_CACHE) + " " + str(64) + " " + str(0) + " " + str(0) + " \n"
      outrnd = "s " + str(VU_CACHE) + " " + CUR_BACKGROUND + "\n"
            clientSend(conn,outrnd)
#      outrnd = "r " + str(64) + " " + str(0) + " " + str(0) + " " + str(1) + " " + str(VU_CACHE) + " \n"
#      outrnd = "r " + str(0) + " " + str(128) + " " + str(0) + " " + str(1) + " " + str(VU_CACHE) + " \n"
      outrnd = "r " + CUR_FOREGROUND + " " + str(1) + " " + str(VU_CACHE) + " \n"
      clientSend(conn,outrnd)
      outrnd = "c 1" + " \n"
          clientSend(conn,outrnd)

  if LED_MODE == 1:
      outrnd = "c 0" + " \n"
          clientSend(conn,outrnd)
      outrnd = "s " + str(random.randrange(1,120)) + " " + str(random.randrange(0,255)) + " " + str(random.randrange(0,255)) + " " + str(random.randrange(0,255)) + " \n"
          clientSend(conn,outrnd)
      outrnd = "c 1" + " \n"
          clientSend(conn,outrnd)
      LED_IDX = LED_IDX + 1
      if LED_IDX > 750:
    LED_IDX = 0
    LED_MODE = 0
          changeColor()

  time.sleep(0.02)
    conn.close()

changeColor()

#now keep talking with the client
while 1:
    #wait to accept a connection - blocking call
    conn, addr = s.accept()
    print 'Connected with ' + addr[0] + ':' + str(addr[1])

    #start new thread takes 1st argument as a function name to be run, second is the tuple of arguments to the function.
    start_new_thread(clientthread ,(conn,))

s.close()
