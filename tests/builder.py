import sys 
import os
import re 
import tempfile 
import subprocess 
import multiprocessing 
import shutil 
import glob 
import itertools 
import unittest
import zipfile 

class ArduinoBuilder(unittest.TestCase) : 

    tempdir = tempfile.TemporaryDirectory()

    # XXX: These should be configurable. 
    arduino = "/opt/arduino/arduino"
    installdir = '/home/maximus/Arduino/ArduinoGrader'
    testdir = installdir + '/tests'
    sketchbase = '' 
    sketchdir  = '' 

    program = None
    executable = None

    # 
    # Staic accessors 
    #
    def set_program(p) :
        ArduinoBuilder.program = p 
        ArduinoBuilder.sketchbase = os.path.basename(ArduinoBuilder.program)

        # Fix numbered overrides. 
        ArduinoBuilder.sketchbase = re.sub('\s+\(\d+\)', '', ArduinoBuilder.sketchbase)
        # Fix the common .ino.ino problem 
        ArduinoBuilder.sketchbase = ArduinoBuilder.sketchbase.replace('.ino.ino', '.ino')
        ArduinoBuilder.sketchdir = ArduinoBuilder.sketchbase.replace('.ino', '')

        os.makedirs(os.path.join(ArduinoBuilder.tempdir.name, ArduinoBuilder.sketchdir, "build"))
        os.makedirs(os.path.join(ArduinoBuilder.tempdir.name, 'logs'))
        
        print ("Sketch is:", ArduinoBuilder.sketchbase)

    def get_exe() :
        if ArduinoBuilder.executable is None : 
            ArduinoBuilder.build()
        return ArduinoBuilder.executable

    def get_sketch() :
        return ArduinoBuilder.sketchbase

    def open_log(name, mode) :
        return open(os.path.join(ArduinoBuilder.tempdir.name, 'logs', name), mode)

    def save_logs() :
        zipf = zipfile.ZipFile('logs.zip', 'w', zipfile.ZIP_DEFLATED)
        for root, dirs, files in os.walk(os.path.join(ArduinoBuilder.tempdir.name, 'logs')):
            for f in files:
                zipf.write(os.path.join(root, f), arcname=f)
        zipf.close()
        
    # 
    # Test case... 
    # Executing this test case will build the program
    #
    def test_compile(self) :
        #log = ArduinoBuilder.open_log("build.log", "w")
        log = sys.stdout 

        ncpus = multiprocessing.cpu_count()

        shutil.copy(ArduinoBuilder.program, os.path.join(ArduinoBuilder.tempdir.name, ArduinoBuilder.sketchdir, ArduinoBuilder.sketchbase))

        tempsketch = os.path.join(ArduinoBuilder.tempdir.name, ArduinoBuilder.sketchdir, ArduinoBuilder.sketchbase)
        tempbuild = os.path.join(ArduinoBuilder.tempdir.name, "build")

        subprocess.check_call([ArduinoBuilder.arduino, '--verify', '--preserve-temp-files', '--pref',
                               'build.path=' + tempbuild, tempsketch], stdout=log, stderr=log)

        log.write("\n*** Arduino verify succeeded. ***\n\n")
        log.flush()

        shutil.copy( os.path.join(ArduinoBuilder.tempdir.name, "build", "sketch", ArduinoBuilder.sketchdir + ".ino.cpp"), 
                     os.path.join(ArduinoBuilder.tempdir.name, ArduinoBuilder.sketchdir + ".cpp") )                
                    
        for f in [os.path.join(ArduinoBuilder.installdir, 'emu', 'emulator.make'), 
                  os.path.join(ArduinoBuilder.installdir, 'emu', 'emulator.cpp'), 
                  os.path.join(ArduinoBuilder.installdir, 'emu', 'emulator.h')] + glob.glob(ArduinoBuilder.installdir + "/arduino/*") :
            shutil.copy(f, ArduinoBuilder.tempdir.name)

        subprocess.check_call(['make', '-j', str(ncpus), '-f', 'emulator.make', '-C', ArduinoBuilder.tempdir.name, 'all'], stdout=log, stderr=log)

        log.write("\n*** Native compile succeeded. ***\n")
        log.flush()

        ArduinoBuilder.executable = ArduinoBuilder.tempdir.name + '/test'
        if not os.path.exists(ArduinoBuilder.executable) :
            print ("ERROR: Failed to create executable")
            return False
        
        shutil.copy(ArduinoBuilder.executable, ".")
        #log.close()
        return True
