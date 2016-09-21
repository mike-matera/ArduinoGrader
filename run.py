#! /usr/bin/python3

import sys 
import os
import re 
import tempfile 
import subprocess 
import multiprocessing 
import shutil 
import glob 
import itertools 
import pexpect 
import importlib 

tempdir = tempfile.TemporaryDirectory()

# XXX: This should be configurable. 
installdir = '/home/maximus/Arduino/ArduinoGrader'
testdir = installdir + '/tests'

def build (program) :
    arduino = "/opt/arduino/arduino"
    sketchbase = os.path.basename(program)
    sketchdir = sketchbase.replace('.ino', '')

    # Fix the common .ino.ino problem 
    sketchbase = sketchbase.replace('.ino.ino', '.ino')

    ncpus = multiprocessing.cpu_count()

    os.makedirs(tempdir.name + "/" + sketchdir + "/build")
    shutil.copy(program, tempdir.name + "/" + sketchdir + "/" + sketchbase)

    tempsketch = tempdir.name + "/" + sketchdir + "/" + sketchbase
    tempbuild = tempdir.name + "/build"

    subprocess.check_call([arduino, '--verify', '--preserve-temp-files', '--pref',
                       'build.path=' + tempbuild, tempsketch])

    print ("\n*** Arduino build succeeded. ***\n")

    shutil.copy(tempdir.name + "/build/sketch/" + sketchdir + ".ino.cpp", 
            tempdir.name + "/" + sketchdir + ".cpp") 
    for f in [installdir + '/emulator.make', installdir + '/emu/emulator.cpp', installdir + '/emu/emulator.h'] + glob.glob(installdir + "/arduino/*") :
        shutil.copy(f, tempdir.name)

    #shutil.copytree(tempdir.name, os.getcwd() + "/debug")
    subprocess.check_call(['make', '-j', str(ncpus), '-f', 'emulator.make', '-C', tempdir.name, 'all'])

    print ("\n*** Native compile succeeded. ***\n")

    return tempdir.name + '/test'

if len(sys.argv) != 2 :
    print ("usage:", sys.argv[0], "<program.ino>")
    exit (1)

if not os.path.isfile(sys.argv[1]) :
    print ("Sketch doesn't exist:", sys.argv[1])
    exit (2)

def findtests() :
    tests = []
    print ("Searching for test modules...")
    for e in os.listdir(testdir) : 
        d = os.path.join(testdir,e)
        if os.path.isdir(d) : 
            f = os.path.join(d, '__init__.py')
            if os.path.isfile(f) :
                ns = importlib.import_module("tests." + e)
                print ("\t", ns.name)
                tests.append(ns)
    return tests

#os.environ['PYTHONPATH'] = os.getcwd()    
os.environ['PYTHONPATH'] = installdir
tests = findtests()
testexe = None

allpass = True
testcount = 0

for test in tests : 
    for pattern in test.patterns :
        m = re.search(pattern[0], sys.argv[1])
        if m is not None :
            if testexe == None :
                testexe = build(sys.argv[1])
            print ("Executing", test.name, "/", pattern[2])
            testcount += 1
            if pattern[1](testexe) :
                print (" *** PASSED *** ")
            else:
                print (" *** FAILED *** ")
                allpass = False

if testcount > 0 :
    if allpass :
        print (" *** All tests PASSED **** ")
    else:
        print (" *** Some tests FAILED *** ")
else:
    print (" *** No tests run *** ")

