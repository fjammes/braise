#!/bin/sh

# Build platformIO package

# Pre-requisites:
# - Install https://github.com/platformio/platformio/blob/develop/scripts/99-platformio-udev.rules
# - Initialize project:
#   platformio init --board mzeropro 

platformio run --environment mzeroproUSB --upload-port /dev/ttyACM1 -t upload

# TODO: wait for platformIO fix
#~/.platformio/packages/tool-openocd/bin/openocd  -f /home/lora/.platformio/packages/framework-arduinosam/variants/arduino_zero_org/openocd_scripts/arduino_zero_org.cfg -s /home/lora/.platformio/packages/tool-openocd/share/openocd/scripts -s /home/lora/.platformio/packages/tool-openocd/share/openocd/scripts/board -s /home/lora/.platformio/packages/framework-arduinosam/variants/arduino_zero_org/openocd_scripts --command="telnet_port disabled; program {{.pioenvs/mzeropro/firmware.bin}} verify reset 0x4000; shutdown"
