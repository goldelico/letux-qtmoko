#!/bin/bash
# run this script within ssh to qemu
#
# see https://github.com/radekp/qtmoko/blob/master/doc/txt/debian_rootfs_howto_gta04.txt
#

VERSION=57

echo "build image in qemu"
pwd
apt-get update

which fuser >/dev/null || apt-get install -y psmisc
which bzip2 >/dev/null || apt-get install -y bzip2
which partprobe >/dev/null || apt-get install -y parted
which mkfs.vfat >/dev/null || apt-get install -y dosfstools

echo "cleaning up and making new rootfs"
umount /media/rootfs
rm -rf /media/rootfs

# Step 1
# ======

# * We will be installing new image to /media/rootfs:
# * now cdebootstrap a clean base system

which cdebootstrap >/dev/null || apt-get install -y cdebootstrap

INCLUDES+=ifupdown,net-tools,procps,netbase,nano
INCLUDES+=,module-init-tools,wget,openssh-server
INCLUDES+=,screen,mc,libts-0.0-0,libasound2,alsa-utils,udhcpc,wpasupplicant,wireless-tools
INCLUDES+=,iputils-ping,iproute,bluez,bluez-alsa,dosfstools,fbset,rsyslog,psmisc
INCLUDES+=,iptables,dash,ttf-dejavu,libpng12-0,libjpeg8,libxtst6,sqlite3,ntpdate
INCLUDES+=,gstreamer0.10-ffmpeg,gstreamer0.10-plugins-good,gstreamer0.10-fluendo-mp3
INCLUDES+=,liblcms1,libmng1,libtiff4,libvorbisfile3,libicu48,gpsd,gpsd-clients
INCLUDES+=,rfkill,libspeex1,libspeexdsp1,lsof,gstreamer0.10-pulseaudio,pulseaudio
INCLUDES+=,pulseaudio-utils,libasound2-plugins,libpulse-mainloop-glib0

# more for convenience
INCLUDES+=,file # this should be defined in the kernel package as "pre-depends: file"
INCLUDES+=,vi

cdebootstrap --flavour=minimal --include="${INCLUDES}" wheezy /media/rootfs/ http://cdn.debian.net/debian/

mkdir -p /media/rootfs/mnt/nfs
mkdir -p /media/rootfs/mnt/p1
mkdir -p /media/rootfs/mnt/p2
mkdir -p /media/rootfs/mnt/p3
mkdir -p /media/rootfs/mnt/p5
mkdir -p /media/rootfs/mnt/p6
mkdir -p /media/rootfs/mnt/p7
mkdir -p /media/rootfs/media/card
mkdir -p /media/rootfs/media/p1

mkdir -p /media/rootfs/proc /media/rootfs/sys
mount -t proc proc /media/rootfs/proc
mount -t sysfs sys /media/rootfs/sys

echo "chroot and post-install"

chroot /media/rootfs /bin/bash <<END_CHROOT
echo "chroot started"
pwd
cat /etc/debian_version

# Step 2
# ======

# * Complete installation. Copy paste below in shell:

mkdir /mnt/nfs
mkdir /mnt/p1
mkdir /mnt/p2
mkdir /mnt/p3
mkdir /mnt/p5
mkdir /mnt/p6
mkdir /mnt/p7
mkdir /media/p1
echo "neo" > /etc/hostname
echo "127.0.0.1 neo" >> /etc/hosts

cat > /etc/network/interfaces <<__END__
auto lo
iface lo inet loopback
auto usb0
iface usb0 inet static
    address 192.168.0.202
    netmask 255.255.255.0
    network 192.168.0.0
    gateway 192.168.0.200
    up rm /etc/resolv.conf
    up echo nameserver 208.67.222.222 >/etc/resolv.conf
__END__

cat > /etc/fstab << __END__
rootfs  /                auto    defaults,errors=remount-ro,noatime 0 1
/dev/mmcblk0p1 /media/p1   auto  defaults                           0 0
/dev/mmcblk0p5  auto  defaults                           0 0
proc    /proc            proc    defaults                           0 0
#tmpfs   /var/cache/apt   tmpfs   defaults,noatime                   0 0
192.168.0.200:/ /mnt/nfs  nfs noauto,nolock,soft,rsize=32768,wsize=32768 0 0
__END__

cat > /etc/apt/apt.conf.d/99no-install-recommends << __END__
APT::Install-Recommends "0";
__END__
sed -i 's/\(PermitEmptyPasswords\) no/\1 yes/' /etc/ssh/sshd_config

echo root: root | chpasswd
sed -i 's/root:.*/root:C0XOiCyzQDtsA:14973:0:99999:7:::/' /etc/shadow
apt-get --yes --purge remove cdebootstrap-helper-rc.d


# Step 3 - add more package repositories
# ========================================

echo "deb http://cdn.debian.net/debian wheezy contrib" >> /etc/apt/sources.list
echo "deb http://cdn.debian.net/debian wheezy non-free" >> /etc/apt/sources.list
echo "deb http://security.debian.org/ wheezy/updates main contrib non-free" >> /etc/apt/sources.list
mkdir -p /etc/apt/sources.list.d
echo "deb http://qtmoko.sourceforge.net/debian/gta04/armhf /" > /etc/apt/sources.list.d/qtmoko.list
echo "deb http://qtmoko.sourceforge.net/apps/all /" >> /etc/apt/sources.list.d/qtmoko.list
echo "deb http://qtmoko.sourceforge.net/apps/armhf /" >> /etc/apt/sources.list.d/qtmoko.list

# Step 4
# ======

# Device drivers modules. Most of device drivers are built in kernel, but
#  following drivers are exception: g_ether is module so that we can switch
#  between mass storage, ppp_generic is not needed for people not using GPRS,
#  joydev is used in qtmaze for accelerometers, hso is for UMTS.

echo g_ether > /etc/modules
echo joydev >> /etc/modules
echo hso >> /etc/modules
echo ledtrig-timer >> /etc/modules
echo bmp085 >> /etc/modules

# Make the MAC address of g_ether fixed instead of random, so that network
#  manager can be configured:
  
echo "options g_ether host_addr=46:0d:9e:67:69:eb dev_addr=b6:c8:ab:ac:44:7f" >> /etc/modprobe.d/options

# Configure bluez4 for handsfreee calls, otherwise there is no sounds:

sed -ibak 's|# SCORouting=PCM|SCORouting=PCM|g' /etc/bluetooth/audio.conf

# Remove alsa-utils init scripts - we dont need them, we set correct alsa
#  state ourselves when qtmoko starts up:

update-rc.d -f alsa-utils remove

# Step 4.1 - stable device nodes for UMTS
# =======================================

# commented out because it is provided by gta04 kernel package

# Modem device is by default on /dev/ttyHS3, but it can rename after modem
#  disconnect. For details see:
#    http://lists.goldelico.com/pipermail/gta04-owner/2012-February/001515.html
#    http://lists.goldelico.com/pipermail/gta04-owner/2012-February/001563.html
#  We will use udev to create stable symlinks for us:

#cd /etc/udev/rules.d

#wget http://git.goldelico.com/?p=gta04-kernel.git;a=blob;f=GTA04/udev-rules/hso.rules
#wget http://git.goldelico.com/?p=gta04-kernel.git;a=blob;f=GTA04/udev-rules/input.rules
#wget http://git.goldelico.com/?p=gta04-kernel.git;a=blob;f=GTA04/udev-rules/sensor.rules

# Step 4.2 - charging udev rules
# ==============================

# For more info see:
#    http://lists.goldelico.com/pipermail/gta04-owner/2012-April/002272.html

# cat > /etc/udev/rules.d/gta04-charging.rules <<__END__
#SUBSYSTEM=="power_supply", ACTION=="change", DEVPATH=="*power_supply/twl4030_usb" ATTRS{1-0048/twl4030_usb/id}=="floating", ATTR{../../max_current}="500000"
#SUBSYSTEM=="power_supply", ACTION=="change", DEVPATH=="*power_supply/twl4030_usb" ATTRS{1-0048/twl4030_usb/id}=="102k", ATTR{../../max_current}="851000"
#__END__

#wget http://git.goldelico.com/?p=gta04-kernel.git;a=blob;f=GTA04/udev-rules/charging.rules

# Step 4.5 - make it generate SSH keys on start
# =============================================

# SSH server keys are automatically generated during debootstrap. This can be
#  security problem if user does not regenerate them. So we delete old keys and
#  generate new ones during first boot.

rm /etc/ssh/ssh_host_*

cat > /etc/init.d/sshkeysfirstboot << __END__
#!/bin/sh

### BEGIN INIT INFO
# Provides:             sshkeysfirstboot
# Required-Start:       $remote_fs $syslog
# Required-Stop:        $remote_fs $syslog
# Default-Start:        2 3 4 5
# Default-Stop:         0 1 6
# Short-Description:    Generate SSH host keys on first boot
### END INIT INFO

dpkg-reconfigure openssh-server
update-rc.d -f sshkeysfirstboot remove
rm -f /etc/init.d/sshkeysfirstboot

__END__

chmod +x /etc/init.d/sshkeysfirstboot
update-rc.d sshkeysfirstboot defaults


#Step 4.6 - get rid of getty in inittab
#======================================

# TTY's are for logging to console which is useful only if you have USB
#  keyboard. So tty respawning is in done in usb-host.sh script and we can
#  comment it out from /etc/inittab:

sed -ibak 's|\([0-9:]*\)respawn:/sbin/getty 38400 tty|#\1respawn:/sbin/getty 38400 tty|g' /etc/inittab

# configure login on RS232
echo "O2:23:respawn:/sbin/getty -L ttyO2 115200 vt100" >>/etc/inittab

# Step 4.7 - disable logging by defaults
# ======================================

# We will have syslogd and klogd disabled by default. If phone works it's not
#  needed and it saves flash memory from wearing off. It can be always enabled
#  in settings->logging or via devtools menu or with logging_enable.sh script.

update-rc.d -f rsyslog remove

# Step 5
# ======

# Set locale to get rid of apt-get warnings

echo "LANG=C" > /etc/default/locale
echo "LC_ALL=C" >> /etc/default/locale

# Fix incompatible libts version

cd /usr/lib
ln -s libts-0.0.so.0 libts-1.0.so.0

# Remove .udev dir, that confuses udev. This dir is here after instalation and
#  causes some error/warnings during init. Not sure what's the exact problem.

cd /dev
rm -rf .udev

# Remove /etc/network/run and do make dir /etc/network/run
#  Otherwise fstab mounting will fail for this directory

rm /etc/network/run
mkdir /etc/network/run

# Step 6.5 - configure gpsd
# =========================

# We use gpsd and it has to be configured for Freerunner:

#nano /etc/default/gpsd

# Make sure you have these values:

#START_DAEMON="true"
#DEVICES="/dev/ttyO1"
#GPSD_OPTIONS="-b"

sed -ibak 's|START_DAEMON=".*"|START_DAEMON="true"|g' /etc/default/gpsd
sed -ibak 's|DEVICES=".*"|DEVICES="/dev/ttyO1"|g' /etc/default/gpsd
sed -ibak 's|GPSD_OPTIONS=".*"|GPSD_OPTIONS="-b"|g' /etc/default/gpsd

# Step 7.5 - dash
# ===============

# For faster boot (~15s) and more memory (+1MB) we use dash instead of bash.

cd /bin/
rm sh
ln -s dash sh


# Step 7.6 - libertas firmware
# ============================

# already provided by kernel package

# Wifi needs sd8686.bin and sd8686_helper.bin in /lib/firmware:

#wget http://ftp.de.debian.org/debian/pool/non-free/libe/libertas-firmware/libertas-firmware_9.70.7.p0.0-1_all.deb
#dpkg -i libertas-firmware_9.70.7.p0.0-1_all.deb
#rm libertas-firmware_9.70.7.p0.0-1_all.deb
#cd /lib/firmware
#mv sd8686.bin 1
#mv sd8686_helper.bin 2
#rm -f *.bin
#mv 1 sd8686.bin
#mv 2 sd8686_helper.bin


# Step 8 - install QtMoko
# =======================

# we don't build the core here

# I use qemu armhf host for building qtmoko package. It should be as simple as

#git checkout v${VERSION}
#dpkg-buildpackage

# Note that you may need to install some dependencies before building:

#apt-get install libX11-dev libXext-dev libXtst-dev libasound2-dev libdbus-1-dev libts-dev libbluetooth-dev

# ...and checkout git submodules as it's necessary to place qt sources into qtmoko/qtopiacore/qt/:

#git submodule update --init

# Please note that releases are made from translations branch so that we have
# all languages.

# Now upload the package to sourceforge. In filezilla connect to sourceforge
#  sftp and upload it to /home/project-web/qtmoko/htdocs/debian/gta04

# Install it

# FIXME: if this is available in the repo, we can just apt-get install qtmoko-gta04

wget http://qtmoko.sourceforge.net/debian/gta04/armhf/qtmoko-gta04_${VERSION}-1_armhf.deb
dpkg -i qtmoko-gta04_${VERSION}-1_armhf.deb
update-rc.d qtmoko-gta04 defaults
rm qtmoko-gta04_${VERSION}-1_armhf.deb

killall qpe.sh                  # recent versions start qtmoko after install, so kill it
killall pulse.sh                # kill pulseaudio - in qemu it just eats cpu

rm -rf /home/root/*  # remove all stuff created by qpe

# relace touch screen by stable node

sed -ibak 's|/dev/input/event0|/dev/input/touchscreen|g' /opt/qtmoko/qpe.env


# Step 8.1 - install mokofaen theme
# =================================

# Mokofaen is now default theme for qtmoko - but it's not part of qtmoko deb
#  package, so that user can uninstall it to save space. So install mokofaen:

wget http://qtmoko.sourceforge.net/apps/all/qtmoko-theme-mokofaen_3-2_all.deb
dpkg -i qtmoko-theme-mokofaen_3-2_all.deb
rm qtmoko-theme-mokofaen_3-2_all.deb

# Step 8.2 - some more alarm and ringtones
# ========================================

mkdir -p /home/root/Documents
(
cd /home/root/Documents
wget https://raw.github.com/radekp/qtmoko/master/src/applications/clock/sounds/Alarm_Beep_01.ogg
wget https://raw.github.com/radekp/qtmoko/master/src/applications/clock/sounds/Alarm_Beep_02.ogg
wget https://raw.github.com/radekp/qtmoko/master/src/applications/clock/sounds/Alarm_Beep_03.ogg
)

# Step 9 - Linux kernel
# =====================

# install a precompiled kernel here (incl. firmware and kernel modules)

# we don't install kernel here - is done later by the makesd script so that we can easily swap/reconfigure

# wget http://download.goldelico.com/gta04/20130412-GTA04-stable-3.7+camera/kernel-image-gta04_0.20130412173501_armel.deb
# dpkg -i kernel-image-gta04_0.20130412173501_armel.deb
# rm kernel-image-gta04_0.20130412173501_armel.deb
#wget http://download.goldelico.com/gta04/debian/dists/stable/main/binary-armhf/quantumstep-linux-gta04nodt-kernel_0.20140521155231_armhf.deb
#dpkg -i quantumstep-linux-gta04nodt-kernel_0.20140521155231_armhf.deb
#rm quantumstep-linux-gta04nodt-kernel_0.20140521155231_armhf.deb


# Step 9.2 - Setup bluetooth uarts
# ================================

# * Bluetooth is on /dev/ttyO0 serial port and we need to run hciattach to have
#  bluetooth working. For more info see http://projects.goldelico.com/p/gta04-kernel/page/Wireless/

echo "-s 3000000 /dev/ttyO0 any 3000000" > /etc/bluetooth/uart


# Step 9.4 - Set pulseaudio as default source/sink for gstreamer
# ==============================================================

. /opt/qtmoko/qpe.env 
gconftool-2 -t string --set /system/gstreamer/0.10/default/audiosink pulsesink
gconftool-2 -t string --set /system/gstreamer/0.10/default/audiosrc pulsesrc

# Step 9.4.1 - config pulseaudio
# ==============================

# * Use speex-fixed-3 for resampling (http://lists.goldelico.com/pipermail/gta04-owner/2012-November/003549.html)
# * Disable udev autodetection, because it's for some reason broken on GTA04:

cd /etc/pulse
wget -O daemon.conf https://raw.github.com/radekp/qtmoko/master/devices/gta04/etc/pulse/daemon.conf
wget -O default.pa https://raw.github.com/radekp/qtmoko/master/devices/gta04/etc/pulse/default.pa

# Step 9.5 - import QtMoko debian repository public keys
# ======================================================

# * We use secure apt and we need keys for QtMoko repositories:

wget https://raw.github.com/radekp/qtmoko/master/dist/qtmoko-pubring.gpg
apt-key add /qtmoko-pubring.gpg 
rm qtmoko-pubring.gpg

# Step 9.6 - setup default DNS
# ============================

# * So that internet works on neo, DNS used during installation is not working
#  e.g. when installing from qemu:

cat > /etc/resolv.conf <<__END__
nameserver 8.8.8.8
__END__


# Step 9.9 - enable ssh access just on usb
# ========================================

# * SSH root access can be dangerous on wifi or on GPRS, so enable it only on usb0

echo "# Enable ssh access just on usb" >> /etc/rc.local
echo "ip6tables -A INPUT ! -i usb0 -p tcp --dport 22 -j REJECT --reject-with tcp-reset; iptables -A INPUT ! -i usb0 -p tcp --dport 22 -j REJECT --reject-with tcp-reset" >> /etc/rc.local

# FIXME:
# Manually edit /media/card/etc/rc.local and move "exit" 0 to the end of file:

# nano /media/card/etc/rc.local
sed -ibak 's|exit 0||g' /etc/rc.local
echo "exit 0" >>/etc/rc.local

# ========================================
# ========================================

rm -rf /usr/share/QuantumSTEP

sync
cat /etc/debian_version
echo "chroot: done"

END_CHROOT

## check for errors

echo "cleanup system"

# Step 9.5.1 - remove bash history
# ================================

# * Remove .bash_history created during install

rm -f /media/rootfs/root/.bash_history

sync
umount /media/rootfs/proc /media/rootfs/sys

# fuser -m /media/rootfs/ -TERM -k
# echo "wait for all processes in chroot to be killed"
# sleep 2
echo "done with building image in qemu"
