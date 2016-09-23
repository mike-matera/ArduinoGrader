import unittest
import subprocess 
import pexpect 
import sys
from tests.builder import ArduinoBuilder

class Part1(unittest.TestCase) :

    def test_serial_input(self) :
        exe = ArduinoBuilder.get_exe()
        test = pexpect.spawnu(' '.join([exe, __name__ + ".prog2", 'part1']))
        #test.logfile = sys.stdout
        test.expect('.*period: (\d+)')
        test.sendline("200")
        test.expect('.*period: 20\d')
        test.terminate()

    def test_serial_lower_limit(self) :
        exe = ArduinoBuilder.get_exe()
        test = pexpect.spawnu(' '.join([exe, __name__ + ".prog2", 'part1']))
        #test.logfile = sys.stdout
        test.expect('.*period: (\d+)')
        test.sendline("50")
        test.expect('.*period: 10\d')
        test.terminate()

    def test_serial_upper_limit(self) :
        exe = ArduinoBuilder.get_exe()
        test = pexpect.spawnu(' '.join([exe, __name__ + ".prog2", 'part1']))
        #test.logfile = sys.stdout
        test.expect('.*period: (\d+)')
        test.sendline("1200")
        test.expect('.*period: 100\d')
        test.terminate()


class Part2(unittest.TestCase) :

    def test_right_button(self) :
        exe = ArduinoBuilder.get_exe()
        test = pexpect.spawnu(' '.join([exe, __name__ + ".prog2", 'part2r']))
        #test.logfile = sys.stdout
        test.expect('.*period: 100\d', timeout=120)
        test.expect('.*period: 100\d')
        test.terminate()

    def test_left_button(self) :
        exe = ArduinoBuilder.get_exe()
        test = pexpect.spawnu(' '.join([exe, __name__ + ".prog2", 'part2l']))
        #test.logfile = sys.stdout
        test.expect('.*period: 10\d', timeout=120)
        test.expect('.*period: 10\d')
        test.terminate()


patterns = [
    ['pro(g|j)2_part1.ino', Part1],
    ['pro(g|j)2_part2.ino', Part1, Part2]
] 

name = "Project 2 Grader"
