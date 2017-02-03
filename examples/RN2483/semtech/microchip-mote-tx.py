#!/usr/bin/env python
# Marten Vijn BSD-license http://svn.martenvijn.nl/svn/LICENSE
#
# http://www.microchip.com/DevelopmentTools/ProductDetails.aspx?PartNO=dm164138
# http://thethingsnetwork.org/wiki/AddressSpace
#
# install pyserial and paho mqtt
# apt-get install mosquitto-clients python-pip python-serial
# pip install paho-mqtt
# debug try::
# mosquitto_sub -v -t "nodes/<yourdevicehere>#" -h croft.thethings.girovito.nl

import time
import serial
import os
import paho.mqtt.publish as publish

writeconfig=1

# your devaddr here
devaddr="0000000"
device="nodes/"+devaddr

def picpu():
  if os.path.isfile(myfile):
    tmp = open(myfile, 'r')
    cputmp=tmp.readline()
    tmp.close()
    cputmp=cputmp.rsplit()
    msg=cputmp[0]
  else:
    msg="aa"
  return msg

def send(data):
	p = serial.Serial("/dev/ttyACM0" , 9600 )
	p.write(data+"\x0d\x0a")
	data.rstrip()
	print(data)
	time.sleep(2)
	rdata=p.readline()
	rdata=rdata[:-1]
	print rdata
# debug
#	publish.single(device, rdata, hostname="croft.thethings.girovito.nl", protocol="publish.MQTTv311")

send("sys reset")
time.sleep(1)

if writeconfig is 1:
  time.sleep(1)
  send("mac set nwkskey 2B7E151628AED2A6ABF7158809CF4F3C")
  send("mac set appskey 2B7E151628AED2A6ABF7158809CF4F3C")
  send("mac set devaddr "+devaddr)
  send("mac set adr on")
  send("mac save")
  time.sleep(5)

send("mac join abp")

while True:
  ## maybe add cputmp here on a pi
  msg=picpu()
#  msg="aa"	
  send("mac tx uncnf 1 "+msg)
  time.sleep(60)
