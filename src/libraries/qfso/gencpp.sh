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
../../../../build/qtopiacore/host/bin/qdbusxml2cpp -p orgfreesmartphonedeviceled specs/xml/org.freesmartphone.Device.LED.xml

echo "Generating org.freesmartphone.GSM.Device.xml"
../../../../build/qtopiacore/host/bin/qdbusxml2cpp -p orgfreesmartphonegsmdevice specs/xml/org.freesmartphone.GSM.Device.xml

echo "Generating org.freesmartphone.GSM.Network.xml"
../../../../build/qtopiacore/host/bin/qdbusxml2cpp -i org.freesmartphonegsmnetworkproviderlist.h -p orgfreesmartphonegsmnetwork specs/xml/org.freesmartphone.GSM.Network.xml

echo "Generating org.freesmartphone.GSM.Call.xml"
../../../../build/qtopiacore/host/bin/qdbusxml2cpp -i org.freesmartphonegsmcalldetaillist.h -p orgfreesmartphonegsmcall specs/xml/org.freesmartphone.GSM.Call.xml
