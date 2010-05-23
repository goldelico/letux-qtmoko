#!/bin/sh
rmmod g_file_storage
mount /media/card
modprobe g_ether
ifup usb0
