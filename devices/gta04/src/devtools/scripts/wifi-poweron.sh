#!/bin/sh
udevd &         # start udevd to supply firmware for wifi
sleep 3
modprobe libertas_sdio
VDD=3150000
# VDD=2800000
echo 1 > /sys/class/gpio/gta04:wlan:cd/value
echo "$VDD" >/sys/devices/platform/reg-virt-consumer.4/max_microvolts
echo "$VDD" >/sys/devices/platform/reg-virt-consumer.4/min_microvolts
echo "normal" >/sys/devices/platform/reg-virt-consumer.4/mode
echo 0 > /sys/class/gpio/gta04:wlan:cd/value  # edge triggers interrupt.