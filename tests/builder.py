import sys 
import os
import re 
import tempfile 
import subprocess 
import multiprocessing 
import shutil 
import glob 
import itertools 

class ArduinoBuilder : 

    tempdir = tempfile.TemporaryDirectory()

    # XXX: This should be configurable. 
    arduino = "/opt/arduino/arduino"
    installdir = '/home/maximus/Arduino/ArduinoGrader'
    testdir = installdir + '/tests'

    program = None
    executable = None

    def set_program(p) :
        ArduinoBuilder.program = p 

    def build() :
        sketchbase = os.path.basename(ArduinoBuilder.program)
        sketchdir = sketchbase.replace('.ino', '')

        # Fix the common .ino.ino problem 
        sketchbase = sketchbase.replace('.ino.ino', '.ino')

        # Fix numbered overrides. 
        sketchbase = sketchbase.replace('\(\d+\)', '')

        ncpus = multiprocessing.cpu_count()

        os.makedirs(ArduinoBuilder.tempdir.name + "/" + sketchdir + "/build")
        shutil.copy(ArduinoBuilder.program, ArduinoBuilder.tempdir.name + "/" + sketchdir + "/" + sketchbase)

        tempsketch = ArduinoBuilder.tempdir.name + "/" + sketchdir + "/" + sketchbase
        tempbuild = ArduinoBuilder.tempdir.name + "/build"

        subprocess.check_call([ArduinoBuilder.arduino, '--verify', '--preserve-temp-files', '--pref',
                               'build.path=' + tempbuild, tempsketch])

        print ("\n*** Arduino build succeeded. ***\n")

        shutil.copy(ArduinoBuilder.tempdir.name + "/build/sketch/" + sketchdir + ".ino.cpp", 
                    ArduinoBuilder.tempdir.name + "/" + sketchdir + ".cpp") 
        for f in [ArduinoBuilder.installdir + '/emu/emulator.make', ArduinoBuilder.installdir + '/emu/emulator.cpp', ArduinoBuilder.installdir + '/emu/emulator.h'] + glob.glob(ArduinoBuilder.installdir + "/arduino/*") :
            shutil.copy(f, ArduinoBuilder.tempdir.name)

        #shutil.copytreet(ArduinoBuilder.empdir.name, os.getcwd() + "/debug")
        subprocess.check_call(['make', '-j', str(ncpus), '-f', 'emulator.make', '-C', ArduinoBuilder.tempdir.name, 'all'])

        print ("\n*** Native compile succeeded. ***\n")

        ArduinoBuilder.executable = ArduinoBuilder.tempdir.name + '/test'

    def get_exe() :
        if ArduinoBuilder.executable is None : 
            ArduinoBuilder.build()
        return ArduinoBuilder.executable
