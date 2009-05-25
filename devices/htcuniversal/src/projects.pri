PROJECTS*=\
    plugins/qtopiacore/kbddrivers/htcbuttons\
    plugins/qtopiacore/kbddrivers/htckeyboard\
    plugins/qtopiacore/kbddrivers/openezxkeypad\
    plugins/audiohardware/htcuniversal


enable_modem {
    for(p,PHONEVENDORS) {
        exists(plugins/phonevendors/$$p/$$tail($$p).pro):PROJECTS*=plugins/phonevendors/$$p
    }
    for(m,MULTIPLEXERS) {
        exists(plugins/multiplexers/$$m/$$tail($$m).pro):PROJECTS*=plugins/multiplexers/$$m
    }
}


