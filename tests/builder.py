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
import glob

class ArduinoBuilder(unittest.TestCase) : 

    # 
    # FIXME: These should be configurable
    #
    arduino = "/opt/arduino/arduino"
    installdir = '/home/maximus/Arduino/ArduinoGrader'
    testdir = '/home/maximus/Arduino/ArduinoGrader/tests'

    def __init__(self, name, context, *args, **kwargs) :
        super().__init__(name, *args, **kwargs)

        self.sketchbase = '' 
        self.sketchdir  = '' 

        self.program = None
        self.executable = None

        self.context = context
        self.set_program(self.context['program'])
        self.context['builder'] = self;

        self.tempdir = os.path.join(self.context['tempdir'], "emu-" + self.sketchbase)
        self.logdir = os.path.join(self.tempdir, 'log')
        self.context['logdir'] = self.logdir;
        if not os.path.isdir(self.logdir) :
            os.makedirs(self.logdir)

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
        return open(os.path.join(self.logdir, name), mode)

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
        '''Running Arduino verify.'''

        ncpus = multiprocessing.cpu_count()

        tempsketch = os.path.join(self.tempdir, self.sketchdir, self.sketchbase)
        if not os.path.isdir(os.path.join(self.tempdir)) :
            os.makedirs(os.path.join(self.tempdir))

        tempbuild = os.path.join(self.tempdir, "build")
        if not os.path.isdir(tempbuild) :
            os.makedirs(tempbuild)

        # If the sketch is saved in an eponymous directory. Copy all contents. 
        if self.sketchdir == self.program.split(os.sep)[-2] :
            shutil.copytree(os.path.dirname(self.program), os.path.dirname(tempsketch))
            for f in glob.glob(os.path.dirname(self.program) + '/*') : 
                shutil.copy(f, self.logdir)
        else:
            os.makedirs(os.path.join(self.tempdir, self.sketchdir))
            shutil.copy(self.program, tempsketch)
            shutil.copy(self.program, self.logdir)

        # Check if there are extra files defined in the test... copy them if they exist
        for extrafile in self.context['extras'] :
            extrafilename = os.path.join(os.path.dirname(self.program), extrafile)
            if os.path.isfile(extrafilename) :
                shutil.copy(extrafilename, os.path.dirname(tempsketch))
                shutil.copy(extrafilename, self.logdir)

        log = self.open_log(self.sketchbase + "-build.log", "w")
        try: 
            subprocess.check_call([ArduinoBuilder.arduino, '--verify', '--preserve-temp-files', '--pref', 'build.path=' + tempbuild, tempsketch], stdout=log, stderr=log)

            log.write("\n*** Arduino verify succeeded. ***\n\n")
            log.flush()

            artifactdir = os.path.join(self.tempdir, "build", "sketch")
            artifacts = glob.glob(os.path.join(artifactdir, "*.h")) + glob.glob(os.path.join(artifactdir, "*.cpp"))
            for artifact in artifacts :
                shutil.copy(artifact, os.path.join(self.tempdir, os.path.basename(artifact)))

            for f in [os.path.join(ArduinoBuilder.installdir, 'emu', 'emulator.make'), 
                      os.path.join(ArduinoBuilder.installdir, 'emu', 'emulator.cpp'), 
                      os.path.join(ArduinoBuilder.installdir, 'emu', 'emulator.h')] + glob.glob(ArduinoBuilder.installdir + "/arduino/*") :
                shutil.copy(f, self.tempdir)

            subprocess.check_call(['make', '-j', str(ncpus), '-f', 'emulator.make', '-C', self.tempdir, 'all'], stdout=log, stderr=log)
            
            log.write("\n*** Native compile succeeded. ***\n")
            log.flush()
            self.executable = self.tempdir + '/test'

        except subprocess.CalledProcessError : 
            log.close()
            self.fail("Arduino verify failed.")

        log.close()
        return True
