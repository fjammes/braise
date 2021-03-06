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

import os
import serial

import time
import paho.mqtt.publish as publish

writeconfig = 1

tty = "/dev/ttyACM0"
baudrate = 57600
endline = "\x0d\x0a"

# LoraWan mote configuration
devaddr = "000000FF"
# optiona for APB, but used by Semtech to trace mote
deveui = "FA9C43C0DE000005"
appskey = "11111111111111111111111111111111"
nwkskey = "11111111111111111111111111111111"

device = "nodes/" + devaddr

myfile = "/tmp/None"


def picpu():
    if os.path.isfile(myfile):
        tmp = open(myfile, 'r')
        cputmp = tmp.readline()
        tmp.close()
        cputmp = cputmp.rsplit()
        msg = cputmp[0]
    else:
        msg = time.strftime("%Y%m%d%H%M%S")
    return msg


def send(data):
    p = serial.Serial(tty, baudrate)
    p.write(data + endline)
    data.rstrip()
    print(data)
    time.sleep(2)
    rdata = p.readline()
    rdata = rdata[:-1]
    print rdata
# debug
# publish.single(device, rdata, hostname="croft.thethings.girovito.nl",
# protocol="publish.MQTTv311")

send("sys factoryRESET")
time.sleep(1)

if writeconfig is 1:
    time.sleep(1)
    send("mac set deveui " + deveui)
    send("mac set nwkskey " + nwkskey)
    send("mac set appskey " + appskey)
    send("mac set devaddr " + devaddr)
    send("mac set adr on")
    send("mac save")
    time.sleep(5)

send("mac join abp")

while True:
    # maybe add cputmp here on a pi
    msg = picpu()
#  msg="aa"
    send("mac tx uncnf 1 " + msg)
    time.sleep(60)
