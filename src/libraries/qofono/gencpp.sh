#!/bin/sh

# Use this script to autogenerate cpp wrappers for dbus calls
#
# We expect that you have built qtmoko in directory build in
# the same folder as qtmoko git.

QDBUSXML2CPP=qofonodbusxml2cpp/qofonodbusxml2cpp
XMLPATH=.

for FILE in $XMLPATH/*ofono*.xml; do
    CLASS=Q`basename $FILE | sed -e' s/.xml// ; s/org.//g ; s/ofono\./OFono/ ; s/\.//g'`
    CPPFILE=`echo $CLASS | sed -e 's/\(.*\)/\L\1/'` 
    echo "Generating class $CLASS from $FILE to $CPPFILE.h/.cpp"
    $QDBUSXML2CPP -p $CPPFILE -c $CLASS $FILE || FAILED="$FAILED $FILE\n"

    #echo -e "\n\n#include \"$CPPFILE.moc\"" >> $CPPFILE.cpp
done

FAILED=`echo $FAILED | sed -e '/^ /d'`
echo -e "\nFollowing files could not be processed due to errors:\n\n $FAILED"

