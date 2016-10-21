#!/bin/sh

# Build platformIO package
platformio run --environment mzeroproUSB --upload-port /dev/ttyACM1 -t upload
