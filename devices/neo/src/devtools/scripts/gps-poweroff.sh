#!/bin/sh
echo "Putting GPS in stand-by mode"
gta02-gps --setup-serial /dev/ttySAC1 --sleep
 
# dump AGPS data if we have fix in 3 seconds from now
if timeout 3 grep -m 1 'GPGSA,A,[23]' /dev/ttySAC1 1>/dev/null 2>&1; then
        echo "We have fix. Dumping AGPS data"
        gta02-gps --verbose --setup-serial /dev/ttySAC1 --dump-agps /var/cache/agpsdata
        exit 0
fi
 
echo "No fix. Not dumping AGPS data"