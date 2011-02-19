script.files=scripts/*
script.path=/bin
script.hint=script
INSTALLS+=script

desktop.files+=desktop/install-apps.desktop
desktop.files+=desktop/install-games.desktop

desktop.path=/apps/Applications
desktop.depends+=install_docapi_f_dir
desktop.hint=desktop
INSTALLS+=desktop

pics.files=*.png\
           *.svg
pics.path=/pics/pkginstall
pics.hint=pics
INSTALLS+=pics
