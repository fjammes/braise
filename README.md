# Pre-requisites

```
# Install third-party libraries
mkdir -p $HOME/Arduino
cd $HOME/Arduino
ln -s $HOME/src/braise/libraries
# Install logging library
cd libraries
git clone https://github.com/fjammes/ArduinoLogging
git checkout arduini-m0-pro
```

# gps_bme

## gps_bme_receive
Récupère les info gps et bme au format json et les décode

## gps_bme_send
Acquisition capteur, json, transmission

# gps_bme_ack

## gps_bme_receive_ack
Reçoit les info gps et bme au format json et les décode, avec ack

## gps_bme_send_ack
Acquisition capteur, json, transmission avec ack

