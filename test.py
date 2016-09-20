#! /usr/bin/python3

import sys 
import os
import arduino

if len(sys.argv) != 3 :
    print ("usage:", sys.argv[0], "<program.ino> <test.py>")
    exit (1)

if not os.path.isfile(sys.argv[1]) :
    print ("Sketch doesn't exist:", sys.argv[1])
    exit (2)

if not os.path.isfile(sys.argv[2]) :
    print ("Test doesn't exist:", sys.argv[2]);
    exit (2)

test = arduino.build(sys.argv[0], sys.argv[1])
foo = test.expect('.*TEST PASSED.*')
print ("I got: ", foo)
