#!/bin/bash

[ -r vmlinuz-3.2.0-4-vexpress ] || curl -O http://download.goldelico.com/gta04/debian/qemu-wheezy-vexpress/vmlinuz-3.2.0-4-vexpress
[ -r initrd.img-3.2.0-4-vexpress ] || curl -O http://download.goldelico.com/gta04/debian/qemu-wheezy-vexpress/initrd.img-3.2.0-4-vexpress
[ -r wheezy.img -o -r wheezy.img.bz2 ] || curl -O http://download.goldelico.com/gta04/debian/qemu-wheezy-vexpress/wheezy.img.bz2
[ -r wheezy.img.bz2 ] && bunzip2 -f wheezy.img.bz2

if pgrep qemu-system-arm
then
	echo "qemu-system-arm already running"
	UFLAG=false
	DFLAG=false
else
	echo "start new qemu-system-arm"
	UFLAG=true
	DFLAG=true
fi >&2

while true
do
	case "$1" in
		-u )	# don't shut down (keep up)
			DFLAG=false
			shift
			;;
		-d )	# force shutdown
			DFLAG=true
			shift
			;;
		* )
			break;
	esac
done

if $UFLAG
then
	echo "starting qemu in background"

	qemu-system-arm -M vexpress-a9 \
		-kernel vmlinuz-3.2.0-4-vexpress \
		-initrd initrd.img-3.2.0-4-vexpress \
		-sd wheezy.img \
		-append "root=/dev/mmcblk0p2" \
		-redir tcp:22222::22 -m 256 \
		-serial none \
		-net nic,vlan=0 -net user,smb=/Users/hns &
	PID=$!

	echo "sleeping 300 seconds" # virtual machine will need quite some time to start up...
	for i in 1 2 3 4 5 6 7 8 9 10
	do
		for j in 1 2 3 4 5 6 7 8 9 10
		do
			sleep 3
			echo -n "."
		done
		echo
	done
fi >&2

ssh -o StrictHostKeyChecking=no -p 22222 root@localhost "$@"

if $DFLAG
then
	echo "stopping qemu"
	ssh -o StrictHostKeyChecking=no -p 22222 root@localhost sh -c 'cd; poweroff'
	sleep 20
	kill $PID
fi >&2
