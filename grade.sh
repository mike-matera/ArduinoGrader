#!/bin/bash 

if [ ! -f "$1" ]; then 
    echo "File not found: $1"
    exit 1
fi 

gedit "$1" > /dev/null 2>&1 & 

graderhome=$(dirname $0)
$graderhome/run.py $1 

echo "Hit enter to continue..."
read DUMMY
