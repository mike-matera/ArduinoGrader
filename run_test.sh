#!/bin/bash 

PATH="/opt/arduino-1.6.5:${PATH}"

set -e

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

tempdir=$(mktemp -d emulator-$sketchbase-$testbase-XXXXXXXXXX)

mkdir -p $tempdir/$sketchdir/build
cp $1 $tempdir/$sketchdir
arduino --verify --preserve-temp-files --pref build.path=$tempdir/build $tempdir/$sketchdir/$sketchbase
cp $tempdir/build/${sketchdir}.cpp $tempdir

cp makefile *.cpp *.h $tempdir
cp $2 $tempdir 

make -C $tempdir all 
./$tempdir/test

rm -rf $tempdir
