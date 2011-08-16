#!/bin/sh
# I am using this script to format qtmoko source code. You will need to install
# patched GNU indent (it's under src/tools/indent).
indent -nut -nbad -bap -nbad -nbc -bbo -hnl -br -bls -c33 -cd33 -ncdb -ce -ci4 -cli0 -d0 -di1 -nfc1 -i4 -ip0 -l80 -lp -npcs -nprs -npsl -sai -saf -saw -ncs -nsc -sob -nfca -cp33 -ss -ts4 -il0 $@


#kdialog --title Test -msgbox "$@"