#!/bin/sh
. /opt/qtmoko/qpe.env

if [ ! -d /dev/pts ]; then
    # running on devtmpfs
    mkdir -p /dev/pts
    mount -t devpts none /dev/pts
    modprobe joydev
fi
rm -f /dev/input/mice
echo '1' > /proc/sys/kernel/printk
echo 0 > /sys/devices/virtual/graphics/fbcon/cursor_blink
stty -F /dev/tty1 -echo
rm -rf /var/run/ppp
mkdir /var/run/ppp
mkdir -p /var/cache/apt/archives/partial
atd /var/spool/at

touch /tmp/restart-qtopia
while [ -e /tmp/restart-qtopia ]; do
qpe 2>&1 | logger -t 'Qtopia'
done