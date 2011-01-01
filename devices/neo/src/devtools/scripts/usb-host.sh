#!/bin/sh
echo 1 > /sys/devices/platform/s3c2440-i2c/i2c-0/0-0073/pcf50633-gpio.0/reg-fixed-voltage.2/gta02-pm-usbhost.0/power_on
echo host > /sys/devices/platform/s3c2410-ohci/usb_mode
