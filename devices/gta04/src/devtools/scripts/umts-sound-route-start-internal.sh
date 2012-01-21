#!/bin/sh
#arecord -fS16_LE -r8000 | aplay -Dhw:1,0 &
#arecord -Dhw:1,0 -fS16_LE -r8000 | aplay &
#sox -r 8000 -t alsa hw:0 -t alsa -r 8000 hw:1 &
#sox -r 8000 -t alsa hw:1 -t alsa -r 8000 hw:0 &

#sox -r 8000 -t alsa snd8k -t alsa -r 8000 hw:1 &
#sox -r 8000 -t alsa hw:1 -t alsa -r 8000 snd8k &

#arecord -B 2000 -fS16_LE -r8000 | aplay -B 2000 -Dhw:1,0 &
#arecord -Dhw:1,0 -fS16_LE -r8000 | aplay -B 100000 &

arecord -fS16_LE | aplay -Dhw:1,0 &
arecord -Dhw:1,0 -fS16_LE | aplay
