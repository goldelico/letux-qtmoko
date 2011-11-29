#!/bin/sh
echo "255" >/sys/class/leds/tca6507:6/brightness # activate reset
echo "2800000" >/sys/devices/platform/reg-virt-consumer.4/max_microvolts
echo "2800000" >/sys/devices/platform/reg-virt-consumer.4/min_microvolts
echo "normal" >/sys/devices/platform/reg-virt-consumer.4/mode
echo "0" >/sys/class/leds/tca6507:6/brightness # release reset

hciattach -s 115200 /dev/ttyS0 any 115200 flow
/etc/init.d/bluetooth start

echo "Please restart QtMoko to make bluetooth working"
sleep 5