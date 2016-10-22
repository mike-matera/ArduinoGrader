import unittest
import subprocess 
import pexpect 
import sys

from tests.base import GraderBase 

class Part1(GraderBase) :

    def test_squeeze(self) :
        '''Testing your resistance calculation.'''
        with self.run_test(__name__ + ".prog3", timeout=1) as test :
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

files = [
    ['resistor_calc.ino', Part1],
]

name = "Project 3 Grader"
