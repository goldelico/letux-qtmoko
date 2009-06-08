#!/bin/sh

if [ -e /sys/bus/platform/devices/neo1973-pm-gps.0 ]; then
  echo 0 > /sys/bus/platform/devices/neo1973-pm-gps.0/power_on
fi
if [ -e /sys/class/i2c-adapter/i2c-0/0-0073/neo1973-pm-gps.0/pwron ]; then
  echo 0 > /sys/class/i2c-adapter/i2c-0/0-0073/neo1973-pm-gps.0/pwron
fi
