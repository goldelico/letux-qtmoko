/****************************************************************************
**
** Copyright (C) 2000-2007 TROLLTECH ASA. All rights reserved.
**
** This file is part of the Opensource Edition of the Qtopia Toolkit.
**
** This software is licensed under the terms of the GNU General Public
** License (GPL) version 2.
**
** See http://www.trolltech.com/gpl/ for GPL licensing information.
**
** Contact info@trolltech.com if any conditions of this licensing are
** not clear to you.
**
**
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#define QPE_VERSION "Xanadux-team"
#define QTOPIA_VERSION 0x040300
#define QTOPIA_VERSION_STR 0x040300
#define QTOPIA_AT_MANUFACTURER "QUALCOMM"
#define QTOPIA_AT_REVISION "QUALCOMM"
#ifndef QT_QWS_HTCUNIVERSAL
#define QT_QWS_HTCUNIVERSAL
#endif

#if defined(__GNUC__) && (__GNUC__ > 2)
#define QPE_USE_MALLOC_FOR_NEW
#endif

#define QPE_NEED_CALIBRATION

#define QTOPIA_PHONE_DEVICE "/dev/ttyS0"
#define QTOPIA_PHONE_VENDOR "htcuniversal"

#define QTOPIA_ENABLE_EXPORTED_BACKGROUNDS
#define QTOPIA_ENABLE_GLOBAL_BACKGROUNDS
#define NO_VISUALIZATION
#define QGLOBAL_PIXMAP_CACHE_LIMIT 2097152

// Define the devices whose packages are compatible with this device,
// by convention the first device listed is this device.
#define QTOPIA_COMPATIBLE_DEVICES "htcuniversal"

// Define the name of the Video4Linux device to use for the camera.
// Disabled until we have a working driver ...
/* #ifndef V4L_VIDEO_DEVICE */
/* #define V4L_VIDEO_DEVICE            "/dev/video" */
/* #endif */
