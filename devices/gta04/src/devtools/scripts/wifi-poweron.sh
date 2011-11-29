#!/bin/sh

# Start temporarily udevd so that it provides firmware for the wifi
udevd --debug &
pid=$!

sleep 1

modprobe libertas_sdio

echo "255" >/sys/class/leds/tca6507:6/brightness # activate reset
echo "2800000" >/sys/devices/platform/reg-virt-consumer.4/max_microvolts
echo "2800000" >/sys/devices/platform/reg-virt-consumer.4/min_microvolts
echo "normal" >/sys/devices/platform/reg-virt-consumer.4/mode
echo "0" >/sys/class/leds/tca6507:6/brightness # release reset

sleep 3

# We can stop udevd now
kill -TERM $pid

sleep 1

ifconfig wlan0 up
