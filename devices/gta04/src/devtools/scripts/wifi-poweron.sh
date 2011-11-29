#!/bin/sh
modprobe libertas_sdio

echo "255" >/sys/class/leds/tca6507:6/brightness # activate reset
echo "2800000" >/sys/devices/platform/reg-virt-consumer.4/max_microvolts
echo "2800000" >/sys/devices/platform/reg-virt-consumer.4/min_microvolts
echo "normal" >/sys/devices/platform/reg-virt-consumer.4/mode
echo "0" >/sys/class/leds/tca6507:6/brightness # release reset

# Load firmware, see e.g. http://lwn.net/Articles/242046/
# Normally udev is doing this, but since we dont use it in qtmoko we have to
# provide the firmware manually
sleep 1
if [ -c "/sys/devices/platform/mmci-omap-hs.1/mmc_host/mmc1/mmc1:0001/mmc1:0001:1/firmware/mmc1:0001:1/loading" ]
then
    echo "Supplying wifi firmware";

    echo 1 > /sys/devices/platform/mmci-omap-hs.1/mmc_host/mmc1/mmc1:0001/mmc1:0001:1/firmware/mmc1:0001:1/loading
    cat /lib/firmware/sd8686_helper.bin > /sys/devices/platform/mmci-omap-hs.1/mmc_host/mmc1/mmc1:0001/mmc1:0001:1/firmware/mmc1:0001:1/data
    echo 0 > /sys/devices/platform/mmci-omap-hs.1/mmc_host/mmc1/mmc1:0001/mmc1:0001:1/firmware/mmc1:0001:1/loading

    sleep 1

    echo 1 > /sys/devices/platform/mmci-omap-hs.1/mmc_host/mmc1/mmc1:0001/mmc1:0001:1/firmware/mmc1:0001:1/loading
    cat /lib/firmware/sd8686.bin > /sys/devices/platform/mmci-omap-hs.1/mmc_host/mmc1/mmc1:0001/mmc1:0001:1/firmware/mmc1:0001:1/data
    echo 0 > /sys/devices/platform/mmci-omap-hs.1/mmc_host/mmc1/mmc1:0001/mmc1:0001:1/firmware/mmc1:0001:1/loading
fi
