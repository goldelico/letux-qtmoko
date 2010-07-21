#!/bin/sh
stty -F /dev/ttySAC1 raw
gta02-gps gta02-agps-dump /var/cache/agpsdata < /dev/ttySAC1 > /dev/ttySAC1