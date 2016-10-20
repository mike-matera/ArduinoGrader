import unittest
import subprocess 
import pexpect 
import sys

class Part1(unittest.TestCase) :

    def __init__(self, name, context, *args, **kwargs) :
        super().__init__(name, *args, **kwargs)
        self.context = context;

    def test_squeeze(self) :
        '''Testing your resistance calculation.'''
        exe = self.context['builder'].get_exe()
        test = pexpect.spawnu(' '.join([exe, __name__ + ".prog3"]), timeout=1)
        try :
            test.expect([
                '(?i)resistance\s*:?\s*(\d+)', 
                '(?i)is\s*:?\s*(\d+)', 
                '(?i)(\d+)(\.\d+)?\s*ohms'
             ])
        except pexpect.TIMEOUT :
            self.fail("I never saw you say the word \"resistance\" or a colon or \"ohms\".")

        r = int(test.match.group(1))
        self.assertEqual(r, 26056, msg="Bad resistance value: " + test.match.group(1))
        test.terminate()

files = [
    ['resistor_calc.ino', Part1],
]

name = "Project 3 Grader"
