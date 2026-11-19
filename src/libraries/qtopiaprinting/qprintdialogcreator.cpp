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

// The original Qt Extended build installed its mobile print dialog via
// _qt_print_dialog_creator, a private hook in the bundled QtGui library.
// Stock Qt 4.8 does not export that hook.  Keep the normal Qt print dialog
// rather than referencing a non-existent private symbol at link time.
// The custom mobile dialog can be reintroduced through a public API later.
