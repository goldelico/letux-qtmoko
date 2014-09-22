#!/bin/bash
if pgrep qemu-system-arm
then
	echo "already running"
	ssh -p 22222 root@localhost
else
	echo "starting qemu in background"
	qemu-system-arm -M vexpress-a9 -kernel vmlinuz-3.2.0-4-vexpress -initrd initrd.img-3.2.0-4-vexpress -sd wheezy.img -append "root=/dev/mmcblk0p2" -redir tcp:22222::22 -m 256 -serial none -net nic,vlan=0 -net user,smb=/Users/hns &
	echo "sleeping 300 seconds"
	for i in 1 2 3 4 5 6 7 8 9 10
	do
		for j in 1 2 3 4 5 6 7 8 9 10
			do
				sleep 3	# virtual machine will need quite some time to start up...
				echo -n "."
			done
		echo
	done
	ssh -o StrictHostKeyChecking=no -p 22222 root@localhost
	echo "stopping qemu"
	sleep 20
	killall qemu-system-arm
fi
