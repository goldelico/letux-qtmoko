#!/bin/sh
rfkill unblock wwan
sleep 3 # wait some time for modem to appear (let udev scripts run)
exit 0
