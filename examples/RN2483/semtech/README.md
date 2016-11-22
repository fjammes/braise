lora@clrinfopc04:~/src/braise/examples/RN2483$ ./connect.sh /dev/ttyACM0 57600

Then stty -F /dev/ttyACM0 57600 -cstopb -parity
sys factoryRESET
RN2483 1.0.1 Dec 15 2015 09:38:09^M
sys get ver
RN2483 1.0.1 Dec 15 2015 09:38:09^M
mac set deveui FA9C43C0DE000001
ok^M
mac set appkey 5036e96a74aaaaabafc916958b555554
ok^M
mac set appeui FA9C43C0DA000001
ok^M
mac save
ok^M
mac join otaa
ok^M

