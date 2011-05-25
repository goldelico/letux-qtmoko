/*
 * This file was generated by qfsodbusxml2cpp version 0.7
 * Command line was: qfsodbusxml2cpp -p qfsogsmsms -c QFsoGSMSMS specs/xml/org.freesmartphone.GSM.SMS.xml
 *
 * qfsodbusxml2cpp is Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#include "qfsogsmsms.h"

/*
 * Implementation of interface class QFsoGSMSMS
 */

QFsoGSMSMS::QFsoGSMSMS(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent)
    : QDBusAbstractInterface(service, path, staticInterfaceName(), connection, parent)
{
    qRegisterMetaType<QFsoSIMMessage>("QFsoSIMMessage");
    qDBusRegisterMetaType<QFsoSIMMessage>();
    qRegisterMetaType<QFsoSIMMessageList>("QFsoSIMMessageList");
    qDBusRegisterMetaType<QFsoSIMMessageList>();
}

QFsoGSMSMS::~QFsoGSMSMS()
{
}

