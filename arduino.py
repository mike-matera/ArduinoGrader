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

    print ("\n*** Arduino compile succeeded. ***\n")

    shutil.copy(tempdir.name + "/build/sketch/" + sketchdir + ".ino.cpp", 
            tempdir.name + "/" + sketchdir + ".cpp") 
    for f in ['emulator.make'] + glob.glob("emu/*") + glob.glob("arduino/*") :
        shutil.copy(f, tempdir.name)
    
    subprocess.check_call(['make', '-j', str(ncpus), '-f', 'emulator.make', '-C', tempdir.name, 'all'])

    print ("\n*** Native compile succeeded. ***\n")

    os.environ['PYTHONPATH'] = os.getcwd()
    subprocess.check_call([tempdir.name + '/test', 'pytest.' + test])
#    test = pexpect.spawn('./test')
#    return test

