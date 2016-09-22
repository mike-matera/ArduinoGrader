import unittest
import subprocess 
import pexpect 
import sys
from tests.builder import ArduinoBuilder

class Part1(unittest.TestCase): 

    def test_any_frequency(self) :
        exe = ArduinoBuilder.get_exe()
        test = pexpect.spawnu(' '.join([exe, __name__ + ".prog1"]))
        test.logfile = sys.stdout
        test.expect('.*frequency OK')

class Part2(unittest.TestCase):

    def test_200_400(self) :
        exe = ArduinoBuilder.get_exe()
        test = pexpect.spawnu(' '.join([exe, __name__ + ".prog1"]))
        test.logfile = sys.stdout
        test.expect('.*200')
        test.expect('.*400')
        test.expect('.*frequency OK')

patterns = [
    ['prog1_part1.ino', Part1],
    ['prog1_part2.ino', Part2]
] 

name = "Project 1 Grader"
