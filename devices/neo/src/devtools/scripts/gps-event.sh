#!/bin/sh

GPS_DATA_DIR="/var"
GPS_DEVICE="/dev/ttySAC1"
AGPS_PATH="$GPS_DATA_DIR/agpsdata"
STATE_PATH="$GPS_DATA_DIR/user-state"
GPS_ARGS="--verbose --setup-serial $GPS_DEVICE"

# let om work
. /opt/qtmoko/qpe.env

# create state directory if needed
mkdir -p "$GPS_DATA_DIR"

gps_enabled() {
    [ "`om gps power`" = "1" ]
}

load_agps() {
    echo "Supplying AGPS data"
    gta02-gps $GPS_ARGS --load-agps $AGPS_PATH
}

dump_agps() {
    timeout=2
    if timeout "$timeout" grep -m 1 'GPGSA,A,[23]' "$GPS_DEVICE" 1>/dev/null 2>&1; then
        echo "Dumping AGPS data"
        gta02-gps $GPS_ARGS --dump-agps $AGPS_PATH
    else
        echo "No fix in $timeout seconds. Not dumping AGPS data"
    fi
}

enable_gps() {
    echo "Powering GPS on"
    om gps power 1
}

disable_gps() {
    echo "Powering GPS off"
    om gps power 0
}

sleep_gps() {
    echo "Putting GPS in stand-by mode"
    gta02-gps $GPS_ARGS --sleep
}

wake_gps() {
    echo "Waking up GPS"
    gta02-gps $GPS_ARGS --wake-up
}

sync_time() {
    if /sbin/ifconfig | grep -v LOOPBACK | grep UP 1>/dev/null; then
        echo "Network seems present. Update time using ntp"
        timeout 15 ntpdate pool.ntp.org
    fi
}

# main

case "$1" in
    on)
        echo 1 > "$STATE_PATH"
        if gps_enabled; then
            wake_gps
        else
            enable_gps
            sync_time
            load_agps
            om gps keep-on-in-suspend 1
        fi
        ;;
    off)
        echo 0 > "$STATE_PATH"
        sleep_gps
        dump_agps
        ;;
    suspend)
        if gps_enabled; then
            if [ "`cat "$STATE_PATH"`" = "1" ]; then
                sleep_gps
                dump_agps
            else
                dump_agps
                disable_gps
            fi
        fi
        ;;
    resume)
        if gps_enabled; then
            wake_gps
        fi
        ;;
esac

exit 0

