import unittest
import subprocess 
import pexpect 
import sys
from tests.builder import ArduinoBuilder

class Part1(unittest.TestCase): 

    def __init__(self, name, context, *args, **kwargs) :
        super().__init__(name, *args, **kwargs)
        self.context = context;

    def test_any_frequency(self) :
        '''Testing if your sketch blinks'''
        exe = self.context['builder'].get_exe()
        test = pexpect.spawnu(' '.join([exe, __name__ + ".prog1"]), timeout=1)
        test.expect('.*fc \( h: (\d+) l: (\d+) \)', timeout=1)
        self.assertNotEqual(int(test.match.groups()[0]), 0, msg="Your program doesn't seem to be blinking")
        self.assertNotEqual(int(test.match.groups()[1]), 0, msg="Your program doesn't seem to be blinking.")
        test.expect(pexpect.EOF)

class Part2(unittest.TestCase):

    def __init__(self, name, context, *args, **kwargs) :
        super().__init__(name, *args, **kwargs)
        self.context = context;

    def test_double_lowtime(self) :
        '''Testing if your lowTime/highTime ratio'''
        exe = self.context['builder'].get_exe()
        test = pexpect.spawnu(' '.join([exe, __name__ + ".prog1"]), timeout=1)
        test.expect('.*fc \( h: (\d+) l: (\d+) \)', timeout=1)
        self.assertNotEqual(int(test.match.groups()[0]), 0, msg="Your program doesn't seem to be blinking")
        self.assertNotEqual(int(test.match.groups()[1]), 0, msg="Your program doesn't seem to be blinking.")
        self.assertEqual(2*int(test.match.groups()[0]), int(test.match.groups()[1]), msg="Your offTime isn't double your onTime")
        test.expect(pexpect.EOF)

files = [
    ['pro(j|g)1_part1.ino', Part1],
    ['pro(j|g)1_part2.ino', Part2]
] 

name = "Project 1 Grader"
