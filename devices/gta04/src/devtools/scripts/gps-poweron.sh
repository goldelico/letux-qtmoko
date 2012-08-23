#!/bin/sh
# Turn on GPS antenna
rfkill unblock gps

stty 9600 </dev/ttyO1
