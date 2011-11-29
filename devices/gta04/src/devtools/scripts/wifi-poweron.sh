#!/bin/sh
modprobe libertas_sdio
echo "255" >/sys/class/leds/tca6507:6/brightness # activate reset
echo "3150000" >/sys/devices/platform/reg-virt-consumer.4/max_microvolts
echo "3150000" >/sys/devices/platform/reg-virt-consumer.4/min_microvolts
echo "normal" >/sys/devices/platform/reg-virt-consumer.4/mode
echo "0" >/sys/class/leds/tca6507:6/brightness # release reset
