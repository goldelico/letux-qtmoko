/*
 * This file was generated by qfsodbusxml2cpp version 0.7
 * Command line was: qfsodbusxml2cpp -p qfsotimealarm -c QFsoTimeAlarm specs/xml/org.freesmartphone.Time.Alarm.xml
 *
 * qfsodbusxml2cpp is Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#include "qfsotimealarm.h"

/*
 * Implementation of interface class QFsoTimeAlarm
 */

QFsoTimeAlarm::QFsoTimeAlarm(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent)
    : QDBusAbstractInterface(service, path, staticInterfaceName(), connection, parent)
{
    qRegisterMetaType<QFsoWakeupAlarm>("QFsoWakeupAlarm");
    qDBusRegisterMetaType<QFsoWakeupAlarm>();
    qRegisterMetaType<QFsoWakeupAlarmList>("QFsoWakeupAlarmList");
    qDBusRegisterMetaType<QFsoWakeupAlarmList>();
}

QFsoTimeAlarm::~QFsoTimeAlarm()
{
}

