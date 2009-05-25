#!/bin/sh

#################################### COMMON ##########################################################
export QTDIR=$QPEDIR
export QPEDIR=/opt/Qtopia
export QTOPIA_ZONEINFO_PATH=/usr/share/etc/zoneinfo
export PATH=$QPEDIR/bin:/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin
export LD_LIBRARY_PATH=$QPEDIR/lib:/lib:/usr/lib:/usr/local/lib
export QWS_MOUSE_PROTO="tslib:/dev/input/touchscreen0"
export TSLIB_CONFFILE=/etc/ts.conf
#export QWS_MOUSE_PROTO=linuxtp:/dev/input/event4
export USER=root
export HOME=/home/$USER
if [ -r $QTOPIA_DIR/lib/helix ]; then
    export HELIX_LIBS=$QTOPIA_DIR/lib/helix
fi


#echo '1' > /proc/sys/kernel/printk

################################### DEVICE SELECTOR ##################################################

. /etc/init.d/functions

case `cpuinfo_id` in

        "HTC Universal")
                export QWS_DISPLAY="VNC:Transformed:LinuxFb:mmWidth54:mmHeight75:Rot0"
                export QWS_SIZE="480x640"
                export QWS_KEYBOARD="htcbuthandler:/dev/input/event0 htckbdhandler:/dev/input/event1 htcbuthandler:/dev/input/event2"
                export QTOPIA_PHONE_VENDOR="htcuniversal"
                export QTOPIA_PHONE_DEVICE="/dev/ttyS0:115200"
                ;;
        "HTC Magician")
                export QWS_DISPLAY="VNC:Transformed:LinuxFb:mmWidth44:mmHeight58"
                export QWS_SIZE="240x320"
                export QWS_KEYBOARD="htcbuthandler:/dev/input/event0,GPIO"
#                export QTOPIA_PHONE_VENDOR="htcphone"
#                export QTOPIA_PHONE_DEVICE="/dev/ttyS1:115200"
                export QTOPIA_PHONE_VENDOR=no
                export QTOPIA_PHONE_DEVICE=no
                ;;
        "Motorola Ezx Platform")
                export QWS_DISPLAY="VNC:Transformed:LinuxFb:mmWidth38:mmHeight51"
                export QWS_SIZE="240x320"
                export QWS_KEYBOARD="openezxkbdhandler:/dev/input/event1 /dev/input/event0"
#                export QTOPIA_PHONE_VENDOR="ficgta01"
#                export QTOPIA_PHONE_DEVICE="/dev/mux0:115200"
                export QTOPIA_PHONE_VENDOR=no
                export QTOPIA_PHONE_DEVICE=no
                ;;
        *)
                ;;
esac

################################### COMMON ##################################################


        case $1 in
        'start')
            echo " "
            echo "############################################"
            echo "##### Starting Qtopia on `cpuinfo_id` #####"
            echo "############################################"
            touch /tmp/restart-qtopia
                while [ -e /tmp/restart-qtopia ]; do
                $QPEDIR/bin/qpe -terminal 3
            done
            ;;
        'stop')
            echo "###########################################"
            echo "##### Stoping Qtopia on `cpuinfo_id` #####"
            echo "###########################################"
            killall qpe 2>/dev/null
            killall dbmigrate 2>/dev/null
            killall quicklauncher 2>/dev/null
            killall mediaserver 2>/dev/null
            killall qdsync 2>/dev/null
            killall sipagent 2>/dev/null
            ;;

        'restart')
            echo "##############################################"
            echo "##### Restarting Qtopia on `cpuinfo_id` #####"
            echo "##############################################"
            killall qpe 2>/dev/null
            killall dbmigrate 2>/dev/null
            killall quicklauncher 2>/dev/null
            killall mediaserver 2>/dev/null
            killall qdsync 2>/dev/null
            killall sipagent 2>/dev/null
            $QPEDIR/bin/qpe -terminal 3
            ;;

        *)
            echo "usage: $0 { start | stop | restart }"

            ;;

        esac

