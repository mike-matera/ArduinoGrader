import unittest
import subprocess 
import pexpect 
import sys
import os
from contextlib import contextmanager
from tests.builder import ArduinoBuilder

class GraderBase(unittest.TestCase): 

    def __init__(self, name, context, *args, **kwargs) :
        super().__init__(name, *args, **kwargs)
        self.context = context;

    @contextmanager
    def run_test(self, *args, **kwargs) :
        try :
            exe = self.context['builder'].get_exe()
        except AttributeError :
            self.fail("Your program didn't compile.")

        test = pexpect.spawnu(exe, [str(i) for i in args], **kwargs)
        log = open(os.path.join(self.context['logdir'], 'serial.log'), 'a')
        test.logfile = log
        yield test
        test.terminate()
        log.close()
