#!/bin/bash

PATH=/opt/arduino-1.6.5:$PATH
arduino=arduino
if [ -n "$WINDIR" ]; then
    arduino='/cygdrive/c/Program Files (x86)/Arduino/arduino_debug.exe'
fi

usage () {
    echo "Usage: $0 <sketch.ino>"
}

echo exe: $0
echo sketch: $1 

if [ -z "$1" ]; then
    usage;
    read DUMMY
    exit 1
fi

if [ ! -f "$1" ]; then
    echo "No such file: $1"
    usage
    read DUMMY
    exit 2
fi

# fixme:
installdir=/home/mimatera/ArduinoGrader

sketchbase=$(basename "$1")
sketchdir=$(basename "$1" .ino)
testbase=${sketchdir,,}_test.cpp

if [ ! -f $installdir/tests/$testbase ]; then
    echo "No test for this file: $testbase"
    read DUMMY
    exit 3
fi

tempdir=$(mktemp -d)

if [ -f /proc/cpuinfo ]; then 
    ncpus=$(grep processor /proc/cpuinfo | wc -l) 
else 
    ncpus=2
fi

gedit "$1" & 

mkdir -p $tempdir/$sketchdir/build
(
    set -e
    echo $tempdir
    cp "$1" $tempdir/$sketchdir

    # fix arduino on Cygwin
    if [ -n "$WINDIR" ]; then
        tempsketch=$(cygpath -w -a $tempdir/$sketchdir/$sketchbase)
        tempbuild=$(cygpath -w -a $tempdir/build)
    else
        tempsketch=$tempdir/$sketchdir/$sketchbase
        tempbuild=$tempdir/build
    fi
    "$arduino" --verify --preserve-temp-files --pref build.path=$tempbuild $tempsketch 
    cp $tempdir/build/${sketchdir}.cpp $tempdir
    cp $installdir/emulator.make $installdir/emulator/* $installdir/arduino/* $tempdir
    cp $installdir/tests/$testbase $tempdir 
    make -j $ncpus -f emulator.make -C $tempdir all 
    cd $tempdir
    ./test
)
rm -rf $tempdir

echo "Hit enter to continue..."
read DUMMY
