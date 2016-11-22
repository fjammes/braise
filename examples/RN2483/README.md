# Webpage

https://staging.thethingsnetwork.org/applications/70B3D57ED00015FC/devices/0004A30B001B9BA7

# Basic command for OTAA join

http://www.microchip.com/forums/m945840.aspx

# Serial communication

lora@clrinfopc04:~/src/braise/examples/RN2483$ ./connect.sh /dev/ttyACM0 57600

Then stty -F /dev/ttyACM0 57600 -cstopb -parity
sys get ver
RN2483 1.0.1 Dec 15 2015 09:38:09^M
sys get hweui
0004A30B001B9BA7^M
^C
lora@clrinfopc04:~/src/braise/examples/RN2483$ ./connect.sh /dev/ttyACM0 57600

Then stty -F /dev/ttyACM0 57600 -cstopb -parity
mac set appeui 70B3D57ED00015FC
ok^M
mac save
ok^M
mac set appkey
invalid_param^M
mac set appkey 068525136DFC00BDC5488CC1397E912B
ok^M
mac save
ok^M
mac show
^C
lora@clrinfopc04:~/src/braise/examples/RN2483$ ./connect.sh /dev/ttyACM0 57600

Then stty -F /dev/ttyACM0 57600 -cstopb -parity
mac show appeui
^C
lora@clrinfopc04:~/src/braise/examples/RN2483$ ./connect.sh /dev/ttyACM0 57600

Then stty -F /dev/ttyACM0 57600 -cstopb -parity
mac get appeui
^C
lora@clrinfopc04:~/src/braise/examples/RN2483$ ./connect.sh /dev/ttyACM0 57600

Then stty -F /dev/ttyACM0 57600 -cstopb -parity
mac get appeui
70B3D57ED00015FC^M
mac get deveui
0000000000000000^M
sys get hweui
0004A30B001B9BA7^M
mac get deveui 0004A30B001B9BA7
invalid_param^M
mac set deveui 0004A30B001B9BA7
ok^M
mac save
ok^M
mac get deveui
^C
lora@clrinfopc04:~/src/braise/examples/RN2483$ ./connect.sh /dev/ttyACM0 57600

Then stty -F /dev/ttyACM0 57600 -cstopb -parity
mac get deveui
0004A30B001B9BA7^M
mac get appeui
70B3D57ED00015FC^M
mac get appkey
invalid_param^M
mac set appkey 068525136DFC00BDC5488CC1397E912B
ok^M
mac save
ok^M
^C

# Change channel availability

mac set ch dcycle 0 9
ok^M
mac set ch dcycle 1 99
ok^M
mac set ch dcycle 2 999
ok^M
mac save
ok^M
mac join otaa
no_free_ch^M

