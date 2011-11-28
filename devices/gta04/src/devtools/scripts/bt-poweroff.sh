#!/bin/sh
echo "255" >/sys/class/leds/tca6507:6/brightness # activate reset
echo "0" >/sys/devices/platform/reg-virt-consumer.4/max_microvolts