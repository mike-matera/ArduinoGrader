import re 
import sys
import os 
import tempfile 
import subprocess 
import multiprocessing 
import shutil 
import glob 
import itertools 
import pexpect 

def build (program, test) :
    arduino = "/opt/arduino/arduino"
    if len(sys.argv) != 3 :
        print ("usage:", sys.argv[0], "<program.ino> <test.py>")
        exit (1)

    if not os.path.isfile(sys.argv[1]) :
        print ("Sketch doesn't exist:", sys.argv[1])
        exit (2)

    if not os.path.isfile(sys.argv[2]) :
        print ("Test doesn't exist:", sys.argv[2]);
        exit (2)

    sketchbase = os.path.basename(sys.argv[1])
    sketchdir = sketchbase.replace('.ino', '')
    testbase = os.path.basename(sys.argv[2])

    tempdir = tempfile.TemporaryDirectory()
    ncpus = multiprocessing.cpu_count()

    os.makedirs(tempdir.name + "/" + sketchdir + "/build")
    shutil.copy(sys.argv[1], tempdir.name + "/" + sketchdir)

    tempsketch = tempdir.name + "/" + sketchdir + "/" + sketchbase
    tempbuild = tempdir.name + "/build"

    subprocess.check_call([arduino, '--verify', '--preserve-temp-files', '--pref',
                       'build.path=' + tempbuild, tempsketch])

    print ("\n*** Arduino compile succeeded. ***\n")

    shutil.copy(tempdir.name + "/build/sketch/" + sketchdir + ".ino.cpp", 
            tempdir.name + "/" + sketchdir + ".cpp") 
    for f in [sys.argv[2], 'emulator.make'] + glob.glob("emulator/*") + glob.glob("arduino/*") :
        shutil.copy(f, tempdir.name)

    subprocess.check_call(['make', '-j', str(ncpus), '-f', 'emulator.make', '-C', tempdir.name, 'all'])

    print ("\n*** Native compile succeeded. ***\n")

    os.chdir(tempdir.name)
    test = pexpect.spawn('./test')
    return test

