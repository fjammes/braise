# Pre-requisites

## Setup your system 

* Add user to `dialout` group
* Update udev rules 
```
# Required on Debian 8.6
curl -O https://github.com/platformio/platformio/blob/develop/scripts/99-platformio-udev.rules
sudo cp 99-platformio-udev.rules /etc/udev/rules.d/99-platformio-udev.rules
# Add lines below to 99-platformio-udev.rules
# ATTRS{idVendor}=="2a03", ATTRS{idProduct}=="[08][02]*", ENV{ID_MM_DEVICE_IGNORE}="1"
# ATTRS{idVendor}=="2a03", ATTRS{idProduct}=="004F", ENV{ID_MM_DEVICE_IGNORE}="1"
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

