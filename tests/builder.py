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
    sketchbase = '' 

    program = None
    executable = None

    def set_program(p) :
        ArduinoBuilder.program = p 
        ArduinoBuilder.sketchbase = os.path.basename(ArduinoBuilder.program)

        # Fix the common .ino.ino problem 
        ArduinoBuilder.sketchbase = ArduinoBuilder.sketchbase.replace('.ino.ino', '.ino')

        # Fix numbered overrides. 
        ArduinoBuilder.sketchbase = re.sub('\s+\(\d+\)', '', ArduinoBuilder.sketchbase)
        print ("motherfucking:", ArduinoBuilder.sketchbase)

    def build() :
        sketchdir = ArduinoBuilder.sketchbase.replace('.ino', '')
        ncpus = multiprocessing.cpu_count()

        os.makedirs(os.path.join(ArduinoBuilder.tempdir.name, sketchdir, "build"))
        shutil.copy(ArduinoBuilder.program, os.path.join(ArduinoBuilder.tempdir.name, sketchdir, sketchbase))

        tempsketch = os.path.join(ArduinoBuilder.tempdir.name, sketchdir, sketchbase)
        tempbuild = os.path.join(ArduinoBuilder.tempdir.name, "build")

        subprocess.check_call([ArduinoBuilder.arduino, '--verify', '--preserve-temp-files', '--pref',
                               'build.path=' + tempbuild, tempsketch])

        print ("\n*** Arduino build succeeded. ***\n")

        shutil.copy( os.path.join(ArduinoBuilder.tempdir.name, "build", "sketch", sketchdir + ".ino.cpp"), 
                     os.path.join(ArduinoBuilder.tempdir.name, sketchdir + ".cpp") )

                
                    
        for f in [os.path.join(ArduinoBuilder.installdir, 'emu', 'emulator.make'), 
                  os.path.join(ArduinoBuilder.installdir, 'emu', 'emulator.cpp'), 
                  os.path.join(ArduinoBuilder.installdir, 'emu', 'emulator.h')] + glob.glob(ArduinoBuilder.installdir + "/arduino/*") :
            shutil.copy(f, ArduinoBuilder.tempdir.name)

        #shutil.copytreet(ArduinoBuilder.empdir.name, os.getcwd() + "/debug")
        subprocess.check_call(['make', '-j', str(ncpus), '-f', 'emulator.make', '-C', ArduinoBuilder.tempdir.name, 'all'])

        print ("\n*** Native compile succeeded. ***\n")

        ArduinoBuilder.executable = ArduinoBuilder.tempdir.name + '/test'

    def get_exe() :
        if ArduinoBuilder.executable is None : 
            ArduinoBuilder.build()
        return ArduinoBuilder.executable

    def get_sketch() :
        return ArduinoBuilder.sketchbase
