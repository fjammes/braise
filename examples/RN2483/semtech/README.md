# Serial monitors

```shell
# Validated for ABP
picocom -b 57600 --omap crcrlf --echo /dev/ttyACM0
# Do not work... 
../connect.sh /dev/ttyACM0 57600

# OTAA

Send commands below vie serial monitor, 
output strings are displayed as comment below.

```bash
sys factoryRESET
# RN2483 1.0.1 Dec 15 2015 09:38:09^M
sys get ver
# RN2483 1.0.1 Dec 15 2015 09:38:09^M
mac set deveui FA9C43C0DE000001
# ok
mac set appkey 5036e96a74aaaaabafc916958b555554
# ok
mac set appeui FA9C43C0DA000001
# ok
mac save
# ok
mac join otaa
# ok
```

# APB

```bash
sys factoryRESET
# Optional for ABP, but seems to be required by Semtech Netwrok Server
mac set deveui FA9C43C0DE000005
mac set devaddr 000000FF
mac set appskey 11111111111111111111111111111111
mac set nwkskey 11111111111111111111111111111111
mac set adr on
mac save
mac join abp
```

or, the automated way:

```bash
python microchip-mote-tx.py
```

# Network Server

Sent data is available here:
http://iot.semtech.com/applications/FA-9C-43-C0-DA-00-00-01/data/mote.FA-9C-43-C0-DE-00-00-05/
