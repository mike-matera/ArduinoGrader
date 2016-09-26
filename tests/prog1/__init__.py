import unittest
import subprocess 
import pexpect 
import sys
from tests.builder import ArduinoBuilder

class Part1(unittest.TestCase): 

    def test_any_frequency(self) :
        exe = ArduinoBuilder.get_exe()
        test = pexpect.spawnu(' '.join([exe, __name__ + ".prog1"]))
        #test.logfile = sys.stdout
        test.expect('.*fc \( h: (\d+) l: (\d+) \)', timeout=5)
        self.assertNotEqual(int(test.match.groups()[0]), 0)
        self.assertNotEqual(int(test.match.groups()[1]), 0)
        test.expect(pexpect.EOF)

class Part2(unittest.TestCase):

    def test_double_lowtime(self) :
        exe = ArduinoBuilder.get_exe()
        test = pexpect.spawnu(' '.join([exe, __name__ + ".prog1"]))
        #test.logfile = sys.stdout
        test.expect('.*fc \( h: (\d+) l: (\d+) \)', timeout=5)
        self.assertEqual(2*int(test.match.groups()[0]), int(test.match.groups()[1]))
        test.expect(pexpect.EOF)

patterns = [
    ['pro(j|g)1_part1(-\d+)?.ino', Part1],
    ['pro(j|g)1_part2(-\d+)?.ino', Part2]
] 

name = "Project 1 Grader"
