#!/bin/sh

#   platformio init --board zeroUSB 

# WARN: double click on board button to be able to reload easilly
platformio run -v --environment zeroUSB -t upload

