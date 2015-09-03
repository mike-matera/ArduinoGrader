#!/bin/bash 

PATH="/opt/arduino-1.6.5:${PATH}"

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
    arduino --verify --preserve-temp-files --pref build.path=$tempdir/build $tempdir/$sketchdir/$sketchbase
    cp $tempdir/build/${sketchdir}.cpp $tempdir
    cp emulator.make emulator/* $tempdir
    cp $2 $tempdir 
    make -j $ncpus -f emulator.make -C $tempdir all 
    cd ./$tempdir
    ./test
)
rm -rf $tempdir
