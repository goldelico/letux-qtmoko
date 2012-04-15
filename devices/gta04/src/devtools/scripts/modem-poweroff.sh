#!/bin/sh
if [ -f /sys/class/gpio/gpio186/value ];
then
    for repeat in 1 2 3 4 5
    do
        if [ ! -e /dev/ttyHS_Application ];
        then
                break
        fi
        echo 1 >/sys/devices/virtual/gpio/gpio186/value
        sleep 1
        echo 0 >/sys/devices/virtual/gpio/gpio186/value
        sleep 5
    done
fi