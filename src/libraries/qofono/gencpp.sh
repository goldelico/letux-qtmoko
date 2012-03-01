#!/bin/sh

# Use this script to autogenerate cpp wrappers for dbus calls
#
# We expect that you have built qtmoko in directory build in
# the same folder as qtmoko git.

QDBUSXML2CPP=qofonodbusxml2cpp/qofonodbusxml2cpp
XMLPATH=.

for FILE in $XMLPATH/*ofono*.xml; do
    $QDBUSXML2CPP -p ofonosimmanager ofono.xml org.ofono.SimManager
    $QDBUSXML2CPP -p ofonovoicecallmanager ofono.xml org.ofono.VoiceCallManager
    $QDBUSXML2CPP -p ofonoradiosettings ofono.xml org.ofono.RadioSettings
    $QDBUSXML2CPP -p ofonophonebook ofono.xml org.ofono.Phonebook
    $QDBUSXML2CPP -p ofonomessagemanager ofono.xml org.ofono.MessageManager
    $QDBUSXML2CPP -p ofonopushnotification ofono.xml org.ofono.PushNotification
    $QDBUSXML2CPP -p ofonosupplementaryservices ofono.xml org.ofono.SupplementaryServices
    $QDBUSXML2CPP -p ofononetworkregistration ofono.xml org.ofono.NetworkRegistration
    $QDBUSXML2CPP -p ofonocellbroadcast ofono.xml org.ofono.CellBroadcast
    $QDBUSXML2CPP -p ofonoconnectionmanager ofono.xml org.ofono.ConnectionManager
done
