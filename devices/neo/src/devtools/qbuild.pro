startup.files=startup/qpe.sh \
              startup/qpe.env
startup.path=/
startup.hint=script
INSTALLS+=startup

script.files=scripts/*
script.path=/bin
script.hint=script
INSTALLS+=script

f_dir.files=.directory
f_dir.path=/apps/Devtools
f_dir.trtarget=Devtools
f_dir.hint=desktop nct prep_db
INSTALLS+=f_dir
MODULES*=qtopia::prep_db

desktop.files+=desktop/bt-poweron.desktop
desktop.files+=desktop/bt-poweroff.desktop
desktop.files+=desktop/gps-poweroff.desktop
desktop.files+=desktop/gps-poweron.desktop
desktop.files+=desktop/wifi-bind.desktop
desktop.files+=desktop/wifi-unbind.desktop
desktop.files+=desktop/wifi-suspend-resume.desktop
desktop.files+=desktop/usb-device.desktop
desktop.files+=desktop/usb-host.desktop
desktop.files+=desktop/suspendx.desktop
desktop.files+=desktop/resumex.desktop
desktop.files+=desktop/get-ssh-key.desktop
desktop.files+=desktop/fast-charge.desktop
desktop.files+=desktop/share-gprs.desktop
desktop.files+=desktop/btfix.desktop
desktop.files+=desktop/logging_stop.desktop
desktop.files+=desktop/logging_start.desktop
desktop.files+=desktop/logging_enable.desktop
desktop.files+=desktop/logging_disable.desktop
desktop.files+=desktop/wifi-maxperf-on.desktop
desktop.files+=desktop/wifi-maxperf-off.desktop
desktop.files+=desktop/usb-mass-storage-on.desktop
desktop.files+=desktop/usb-mass-storage-off.desktop

desktop.path=/apps/Devtools
desktop.depends+=install_docapi_f_dir
desktop.hint=desktop
INSTALLS+=desktop

pics.files=*.png\
           *.svg
pics.path=/pics/devtools
pics.hint=pics
INSTALLS+=pics

startup.files=startup/qpe.sh \
              startup/qpe.env

startup.path=/
startup.hint=script
INSTALLS+=startup

help.source=help
help.files=qpe-devtools*
help.hint=help
INSTALLS+=help

