#!/bin/sh
ifdown usb0
umount /media/card
rmmod g_ether
modprobe g_file_storage file=/dev/mmcblk0 stall=0
