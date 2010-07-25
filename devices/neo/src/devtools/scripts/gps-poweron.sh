#!/bin/sh
# let om work
. /opt/qtmoko/qpe.env
 
# enable gps
if [ "`om gps power`" = "0" ]; then
        echo "Powering GPS on"
        om gps power 1
        echo "Supplying AGPS data"
        gta02-gps --verbose --setup-serial /dev/ttySAC1 --load-agps /var/cache/agpsdata
        echo "Setting GPS to be powered in suspend"
        om gps keep-on-in-suspend 1
else
        echo "GPS powered on, waking up"
        gta02-gps --setup-serial /dev/ttySAC1 --wake-up
        echo "Supplying AGPS data"
        gta02-gps --verbose --setup-serial /dev/ttySAC1 --load-agps /var/cache/agpsdata
fi
 
# return OK
exit 0