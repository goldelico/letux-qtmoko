startup.files=startup/qpe.sh \
              startup/qpe.env
startup.path=/
startup.hint=script
INSTALLS+=startup

testenv.files_=$$QTOPIA_DEPOT_PATH/devices/greenphone/src/devtools/startup/qpe_test.env
testenv.path=/
testenv.commands=-$(INSTALL_FILE) $$testenv.files_ $(INSTALL_ROOT)/qpe.env

script.files=scripts/*
script.path=/bin
script.hint=script
INSTALLS+=script

f_dir.files=.directory
f_dir.path=/apps/Devtools
f_dir.trtarget=Devtools
f_dir.hint=desktop nct
INSTALLS+=f_dir

desktop.files=desktop/docapi-rescan.desktop
desktop.files+=desktop/network-services-start.desktop
desktop.files+=desktop/network-services-stop.desktop
desktop.files+=desktop/sdcard-umount.desktop
desktop.files+=desktop/usb-gadget-ether.desktop
desktop.files+=desktop/usb-gadget-stop.desktop
desktop.files+=desktop/get-ssh-key.desktop
desktop.files+=devtools/desktop/sdio-storage.desktop
desktop.files+=desktop/sdio-wifi.desktop
desktop.path=/apps/Devtools
desktop.depends+=install_docapi_f_dir
desktop.hint=desktop
INSTALLS+=desktop

pics.files=*.png\
           *.svg
pics.path=/pics/devtools
pics.hint=pics
INSTALLS+=pics

help.source=$$QTOPIA_DEPOT_PATH/help
help.files=devtools*
help.hint=help
INSTALLS+=help

motd.files=$$QTOPIA_DEPOT_PATH/devices/greenphone/motd.html
motd.path=/etc
INSTALLS+=motd
