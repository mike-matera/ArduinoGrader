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

    # 
    # These should be configurable
    #
    arduino = "/opt/arduino/arduino"
    installdir = '/home/maximus/Arduino/ArduinoGrader'
    testdir = '/home/maximus/Arduino/ArduinoGrader/tests'

    # 
    # Clean up a sketch name
    #
    def clean_sketch(program) :
        sketchbase = os.path.basename(program)

        # Fix numbered overrides. 
        sketchbase = re.sub('\s+\(\d+\)', '', sketchbase)
        # Fix the common .ino.ino problem 
        sketchbase = sketchbase.replace('.ino.ino', '.ino')
        
        return sketchbase

    def __init__(self, name, context, *args, **kwargs) :
        super().__init__(name, *args, **kwargs)

        self.sketchbase = '' 
        self.sketchdir  = '' 

        self.program = None
        self.executable = None

        self.context = context
        self.set_program(self.context['program'])
        context['builder'] = self;

    def set_program(self, p) :
        self.program = p 
        self.sketchbase = os.path.basename(self.program)

        # Fix numbered overrides. 
        self.sketchbase = re.sub('\s+\(\d+\)', '', self.sketchbase)
        # Fix the common .ino.ino problem 
        self.sketchbase = self.sketchbase.replace('.ino.ino', '.ino')
        self.sketchdir = self.sketchbase.replace('.ino', '')

    def get_exe(self) :
        if self.executable is None : 
            self.build()
        return self.executable

    def get_sketch(self) :
        return self.sketchbase

    def open_log(self, name, mode) :
        return open(os.path.join(self.tempdir, 'log', name), mode)

    def save_logs(self) :
        zipf = zipfile.ZipFile('logs.zip', 'w', zipfile.ZIP_DEFLATED)
        for root, dirs, files in os.walk(os.path.join(self.tempdir, 'logs')):
            for f in files:
                zipf.write(os.path.join(root, f), arcname=f)
        zipf.close()
        
    # 
    # Test case... 
    # Executing this test case will build the program
    #
    def test_do_arduino_compile(self) :
        self.tempdir = os.path.join(self.context['tempdir'], "emu-" + self.sketchbase)

        print ("Building sketch:", self.sketchbase, '... ', end='')
        sys.stdout.flush()

        logdir = os.path.join(self.tempdir, 'log')
        if not os.path.isdir(logdir) :
            os.makedirs(logdir)

        log = self.open_log(self.sketchbase + "-build.log", "w")

        ncpus = multiprocessing.cpu_count()

        tempsketch = os.path.join(self.tempdir, self.sketchdir, self.sketchbase)
        if not os.path.isdir(os.path.join(self.tempdir, self.sketchdir)) :
            os.makedirs(os.path.join(self.tempdir, self.sketchdir))

        tempbuild = os.path.join(self.tempdir, "build")
        if not os.path.isdir(tempbuild) :
            os.makedirs(tempbuild)

        shutil.copy(self.program, tempsketch)

        subprocess.check_call([ArduinoBuilder.arduino, '--verify', '--preserve-temp-files', '--pref', 'build.path=' + tempbuild, tempsketch], stdout=log, stderr=log)

        log.write("\n*** Arduino verify succeeded. ***\n\n")
        log.flush()

        shutil.copy(os.path.join(self.tempdir, "build", "sketch", self.sketchdir + ".ino.cpp"), os.path.join(self.tempdir, self.sketchdir + ".cpp"))
                    
        for f in [os.path.join(ArduinoBuilder.installdir, 'emu', 'emulator.make'), 
                  os.path.join(ArduinoBuilder.installdir, 'emu', 'emulator.cpp'), 
                  os.path.join(ArduinoBuilder.installdir, 'emu', 'emulator.h')] + glob.glob(ArduinoBuilder.installdir + "/arduino/*") :
            shutil.copy(f, self.tempdir)

        subprocess.check_call(['make', '-j', str(ncpus), '-f', 'emulator.make', '-C', self.tempdir, 'all'], stdout=log, stderr=log)

        log.write("\n*** Native compile succeeded. ***\n")
        log.flush()

        self.executable = self.tempdir + '/test'
        if not os.path.exists(self.executable) :
            print ("ERROR: Failed to create executable")
            return False
        
        log.close()
        return True
