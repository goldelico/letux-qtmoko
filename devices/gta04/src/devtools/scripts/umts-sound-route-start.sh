#!/bin/sh
arecord -fS16_LE -r8000 | aplay -Dhw:1,0 &
arecord -Dhw:1,0 -fS16_LE -r8000 | aplay &
