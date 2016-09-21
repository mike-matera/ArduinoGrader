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

def build (program, test) :
    arduino = "/opt/arduino/arduino"
    sketchbase = os.path.basename(program)
    sketchdir = sketchbase.replace('.ino', '')

    tempdir = tempfile.TemporaryDirectory()
    ncpus = multiprocessing.cpu_count()

    os.makedirs(tempdir.name + "/" + sketchdir + "/build")
    shutil.copy(program, tempdir.name + "/" + sketchdir)

    tempsketch = tempdir.name + "/" + sketchdir + "/" + sketchbase
    tempbuild = tempdir.name + "/build"

    subprocess.check_call([arduino, '--verify', '--preserve-temp-files', '--pref',
                       'build.path=' + tempbuild, tempsketch])

    print ("\n*** Arduino build succeeded. ***\n")

    shutil.copy(tempdir.name + "/build/sketch/" + sketchdir + ".ino.cpp", 
            tempdir.name + "/" + sketchdir + ".cpp") 
    for f in ['emulator.make', 'emu/emulator.cpp', 'emu/emulator.h'] + glob.glob("arduino/*") :
        shutil.copy(f, tempdir.name)

    #shutil.copytree(tempdir.name, os.getcwd() + "/debug")
    subprocess.check_call(['make', '-j', str(ncpus), '-f', 'emulator.make', '-C', tempdir.name, 'all'])

    print ("\n*** Native compile succeeded. ***\n")

    cmd = [tempdir.name + '/test', 'tests.' + test]
    #subprocess.check_call(cmd)
    child = pexpect.spawnu(' '.join(cmd))
    child.logfile = sys.stdout
    return child

if len(sys.argv) != 3 :
    print ("usage:", sys.argv[0], "<program.ino> <test>")
    exit (1)

if not os.path.isfile(sys.argv[1]) :
    print ("Sketch doesn't exist:", sys.argv[1])
    exit (2)

if not os.path.isfile('tests/' + sys.argv[2] + '.py') :
    print ("Test doesn't exist:", 'tests/' + sys.argv[2] + '.py');
    exit (2)

os.environ['PYTHONPATH'] = os.getcwd()
testmod = importlib.__import__("tests." + sys.argv[2], fromlist=['test_expect'])
test = build(sys.argv[1], sys.argv[2])
if testmod.test_expect(test) :
    print ('\n*** Test passed. ***')
else:
    print ('\n*** Test failed. ***')

