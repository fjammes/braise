# Webpage

https://staging.thethingsnetwork.org/applications/70B3D57ED00015FC/devices/0004A30B001B9BA7

# Basic command for OTAA join

http://www.microchip.com/forums/m945840.aspx

# ABP and OTAA configuration

Check README file in `semtech/`.

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

