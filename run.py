#! /usr/bin/python3

import sys 
import os
import re 
import importlib 
import unittest 
import tempfile
from tests.builder import ArduinoBuilder
from tests.comments import Comments
from tests.runner import runner 
from tests.runner import histogram 

sketchfile = ' '.join(sys.argv[1:])

if len(sys.argv) < 2 :
    print ("usage:", sys.argv[0], "<program.ino>")
    exit (1)

if not os.path.isfile(sketchfile) :
    print ("Sketch doesn't exist:", sketchfile)
    exit (2)

os.environ['PYTHONPATH'] = ArduinoBuilder.installdir

suite = unittest.TestSuite()
loader = unittest.TestLoader()

context = {}
context['tempdir'] = tempfile.TemporaryDirectory().name
context['program'] = sketchfile

print ("Searching for test modules...")
for e in os.listdir(ArduinoBuilder.testdir) :
    d = os.path.join(ArduinoBuilder.testdir,e)
    if os.path.isdir(d) : 
        f = os.path.join(d, '__init__.py')
        if os.path.isfile(f) :
            test = importlib.import_module("tests." + e)
            for pattern in test.files :
                m = re.search(pattern[0], ArduinoBuilder.clean_sketch(sketchfile))
                if m is not None :
                    for tc in ['.*\.ino', ArduinoBuilder, Comments] + pattern[1:] :
                        names = loader.getTestCaseNames(tc)
                        for name in names :
                            suite.addTest(tc(name, context))


runner(suite)
histogram()

#for test in suite : 
#    desc = test.shortDescription()
#    if desc is not None:
#        print (desc, ' ... ', end='', flush=True )
#    result = test.run()
#    if result.wasSuccessful() : 
#        print ("PASS")
#    else:
#        print ("FAIL")
#        print (result.failures[0])

    #print (result)

#ArduinoBuilder.save_logs()
