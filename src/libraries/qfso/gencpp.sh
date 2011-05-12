#!/bin/sh

# Use this script to autogenerate cpp wrappers for dbus calls
#
# We expect that you have built qtmoko in directory build in
# the same folder as qtmoko git.
#
# We also expect that you have built fso xml specs in specs
# subdir

export LD_LIBRARY_PATH=../../../../build/qtopiacore/host/lib/

echo "Generating org.freesmartphone.Device.LED.xml"
../../../../build/qtopiacore/host/bin/qdbusxml2cpp -p org.freesmartphone.Device.LED specs/xml/org.freesmartphone.Device.LED.xml

echo "Generating org.freesmartphone.GSM.Device.xml"
../../../../build/qtopiacore/host/bin/qdbusxml2cpp -p org.freesmartphone.GSM.Device specs/xml/org.freesmartphone.GSM.Device.xml

echo "Generating org.freesmartphone.GSM.Network.xml"
../../../../build/qtopiacore/host/bin/qdbusxml2cpp -p org.freesmartphone.GSM.Network specs/xml/org.freesmartphone.GSM.Network.xml

echo "Generating org.freesmartphone.GSM.Call.xml"
../../../../build/qtopiacore/host/bin/qdbusxml2cpp -p org.freesmartphone.GSM.Call specs/xml/org.freesmartphone.GSM.Call.xml
