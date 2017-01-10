# Pre-requisites

## Setup your system

* Add user to `dialout` group
* Update udev rules
```
# Required on Debian 8.6
curl -O https://github.com/platformio/platformio/blob/develop/scripts/99-platformio-udev.rules
sudo cp 99-platformio-udev.rules /etc/udev/rules.d/99-platformio-udev.rules
```
* Add line below to udev rules (for m0pro and zero)
NOTE: removing modem manager might also be fine
```
sudo apt-get remove modemmanager
```

TODO: remove line below and modemmanager and test again!!!
```
# M0Pro
ATTRS{idVendor}=="2a03", ATTRS{idProduct}=="[08][02]*", ENV{ID_MM_DEVICE_IGNORE}="1"
ATTRS{idVendor}=="2a03", ATTRS{idProduct}=="004F", ENV{ID_MM_DEVICE_IGNORE}="1"
# Braise chip
ATTRS{idVendor}=="2341", ATTRS{idProduct}=="804d", ENV{ID_MM_DEVICE_IGNORE}="1"
ATTRS{idVendor}=="2341", ATTRS{idProduct}=="004d", ENV{ID_MM_DEVICE_IGNORE}="1"
# TODO: test that line
# That line may HELP
SUBSYSTEMS=="usb", ATTRS{idVendor}=="2341", ATTRS{idProduct}=="[80][00]*", MODE:="0666"
```
* Restart udev
```
sudo service udev restart
# Unplug and re-plug arduino board
dmesg
```

## Install platformio

```
sudo pip install -U platformio
```

## Install logging library

```
mkdir -p src
cd src
git clone https://github.com/fjammes/ArduinoLogging
```


TODO: WARN about double-click on arduino board before deploying!!!

# Install BRAISE code

```
git clone git@github.com:fjammes/braise.git
# WARNING: ArduinoLogging is symlinked using hard-coded path
```

# Code architecture

## gps_bme_send

Get gps and bme sensors informations, encode them in json and send them using
lora.


## gps_bme_receive

Retrieve json data using Lora, and print it to serial port.

