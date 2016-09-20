#! /usr/bin/python3

import sys 
import os
import arduino

if len(sys.argv) != 3 :
    print ("usage:", sys.argv[0], "<program.ino> <test>")
    exit (1)

if not os.path.isfile(sys.argv[1]) :
    print ("Sketch doesn't exist:", sys.argv[1])
    exit (2)

if not os.path.isfile('pytest/' + sys.argv[2] + '.py') :
    print ("Test doesn't exist:", 'pytest/' + sys.argv[2] + '.py');
    exit (2)

test = arduino.build(sys.argv[1], sys.argv[2])
#foo = test.expect('.*TEST PASSED.*')
#print ("I got: ", foo)
