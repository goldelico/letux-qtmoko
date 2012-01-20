#!/bin/sh
VDD=3150000
# VDD=2800000
echo 1 > /sys/class/gpio/gta04:wlan:cd/value
echo "$VDD" >/sys/devices/platform/reg-virt-consumer.4/max_microvolts
echo "$VDD" >/sys/devices/platform/reg-virt-consumer.4/min_microvolts
echo "normal" >/sys/devices/platform/reg-virt-consumer.4/mode
echo 0 > /sys/class/gpio/gta04:wlan:cd/value  # edge triggers interrupt.

hciattach -s 115200 /dev/ttyO0 any 115200 flow
hciconfig hci0 up
bccmd -d hci0 psset baudrate 12288
bccmd -d hci0 warmreset
killall hciattach
sleep 3
hciattach -s 3000000 /dev/ttyO0 any 3000000 flow
hciconfig hci0 up

/etc/init.d/bluetooth start

echo "Please restart QtMoko to make bluetooth working"
sleep 5
