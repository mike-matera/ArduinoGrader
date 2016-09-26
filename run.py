#! /usr/bin/python3

import sys 
import os
import re 
import importlib 
import unittest 
from tests.builder import ArduinoBuilder

sketchfile = ' '.join(sys.argv[1:])

if len(sys.argv) < 2 :
    print ("usage:", sys.argv[0], "<program.ino>")
    exit (1)

if not os.path.isfile(sketchfile) :
    print ("Sketch doesn't exist:", sketchfile)
    exit (2)

ArduinoBuilder.set_program(sketchfile)
os.environ['PYTHONPATH'] = ArduinoBuilder.installdir

suite = unittest.TestSuite()
loader = unittest.TestLoader()

suite.addTests(loader.loadTestsFromTestCase(ArduinoBuilder))

print ("Searching for test modules...")
for e in os.listdir(ArduinoBuilder.testdir) :
    d = os.path.join(ArduinoBuilder.testdir,e)
    if os.path.isdir(d) : 
        f = os.path.join(d, '__init__.py')
        if os.path.isfile(f) :
            test = importlib.import_module("tests." + e)
            for pattern in test.patterns :
                m = re.search(pattern[0], ArduinoBuilder.get_sketch())
                if m is not None :
                    for tc in pattern[1:] :
                        suite.addTests(loader.loadTestsFromTestCase(tc))

unittest.TextTestRunner(verbosity=2).run(suite)
ArduinoBuilder.save_logs()
