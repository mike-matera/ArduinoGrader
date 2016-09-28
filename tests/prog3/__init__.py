import unittest
import subprocess 
import pexpect 
import sys

class Part1(unittest.TestCase) :

    def __init__(self, name, context, *args, **kwargs) :
        super().__init__(name, *args, **kwargs)
        self.context = context;

    def test_squeeze(self) :
        exe = self.context['builder'].get_exe()
        test = pexpect.spawnu(' '.join([exe, __name__ + ".prog3"]), timeout=1)
        test.logfile = sys.stdout
        test.expect(['(\d+)\.(\d+)\s*ohms'])
        r = int(test.match.group(1))
        assert(r >= 25000 and r <= 27000)
        test.terminate()

files = [
    ['voltmeter.ino'],
    ['calculator.ino'],
    ['resistor_calc.ino', Part1],
]

name = "Project 3 Grader"
