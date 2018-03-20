/****************************************************************************
**
** This file is part of the Qt Extended Opensource Package.
**
** Copyright (C) 2009 Trolltech ASA.
**
** Contact: Qt Extended Information (info@qtextended.org)
**
** This file may be used under the terms of the GNU General Public License
** version 2.0 as published by the Free Software Foundation and appearing
** in the file LICENSE.GPL included in the packaging of this file.
**
** Please review the following information to ensure GNU General Public
** Licensing requirements will be met:
**     http://www.fsf.org/licensing/licenses/info/GPLv2.html.
**
**
****************************************************************************/

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <custom.h>
#include <sys/stat.h> 
#include <qtopianamespace.h>
#include <QtopiaServiceRequest>
#include <QtopiaIpcEnvelope>
#include <qwindowsystem_qws.h>
#include <QValueSpaceItem>
#include <QValueSpaceObject>
#include <QDebug>

#define BACKLIGHT_PATH "/sys/class/backlight/backlight/"


QTOPIABASE_EXPORT int qpe_sysBrightnessSteps()
{
    int fd = open(BACKLIGHT_PATH "max_brightness", O_RDONLY);
    char buf[16];
    if (fd < 0) {
      perror("cannot open max_brightness, returning default");
      return 100;
    }
    int l = read(fd, buf, sizeof(buf) - 1);
    if (l <= 0) {
      perror("cannot read max_brightness, returning default");
      close(fd);
      return 100;
    }
    close(fd);
    return atoi(buf); 
}

// Write string count bytes long to file
static int write_file(const char *path, const char *value, size_t count)
{
    int fd = open(path, O_WRONLY);
    if (fd < 0)
        return - 1;
    int res = write(fd, value, count);
    close(fd);
    return res;
}


QTOPIABASE_EXPORT void qpe_setBrightness(int b)
{
    char str[8];
    int n = sprintf(str, "%d", b);
    for(int i = 10; i >= 0; i--) {
        if(write_file(BACKLIGHT_PATH "brightness", str, n) == n)
            return;
        perror("qpe_setBrightness failed");
    }
}
