#This file contains projects that make up the InputMethods module.

!x11 {
    PROJECTS*=\
        plugins/inputmethods/predictivekeyboard \
#        plugins/inputmethods/keyboard \
        plugins/inputmethods/fingerkeyboard \
        plugins/inputmethods/dockedkeyboard \
        plugins/inputmethods/transparentkeyboard \
        3rdparty/plugins/inputmethods/pkim \
        plugins/inputmethods/biglandscapekeyboard

    contains(QTOPIA_LANGUAGES,zh_CN):PROJECTS*=plugins/inputmethods/pinyin
}

PROJECTS*=\
    plugins/themeitems/inputmethodsitem
SERVER_PROJECTS*=\
    server/ui/components/inputmethods

PROJECTS*=\
    libraries/handwriting \
    settings/hwsettings \
    3rdparty/libraries/inputmatch \
    settings/words
