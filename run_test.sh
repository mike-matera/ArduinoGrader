#!/bin/bash 

arduino=arduino
if [ -n "$WINDIR" ]; then
    arduino='/cygdrive/c/Program Files (x86)/Arduino/arduino_debug.exe'
fi

usage () {
    echo "Usage: $0 <sketch.ino> <test.cpp>"
    exit -1 
}
 
if [ -z "$1" -o -z "$2" ]; then
    usage
fi

if [ ! -f "$1" ]; then
    echo "No such file: $1"
    exit -2
fi

if [ ! -f "$2" ]; then
    echo "No such file: $2"
    exit -2
fi

sketchbase=$(basename $1)
sketchdir=$(basename $1 .ino)
testbase=$(basename $2)

tempdir=$(mktemp -d build-$sketchbase-$testbase-XXXXXXXXXX)

if [ -f /proc/cpuinfo ]; then 
    ncpus=$(grep processor /proc/cpuinfo | wc -l) 
else 
    ncpus=2
fi

mkdir -p $tempdir/$sketchdir/build
(
    set -e
    cp $1 $tempdir/$sketchdir

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
    cp emulator.make emulator/* $tempdir
    cp $2 $tempdir 
    make -j $ncpus -f emulator.make -C $tempdir all 
    cd ./$tempdir
    ./test
)
rm -rf $tempdir
