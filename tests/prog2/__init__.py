import unittest
import subprocess 
import pexpect 
import sys
from tests.builder import ArduinoBuilder

class Part1(unittest.TestCase) :

    def __init__(self, name, context, *args, **kwargs) :
        super().__init__(name, *args, **kwargs)
        self.context = context;

    def test_does_blink(self) :
        exe = self.context['builder'].get_exe()
        test = pexpect.spawnu(' '.join([exe, __name__ + ".prog2", 'part1']), timeout=2)
        #test.logfile = sys.stdout
        test.expect('.*period: (\d+)')
        test.terminate()

    def test_serial_valid_number(self) :
        exe = self.context['builder'].get_exe()
        test = pexpect.spawnu(' '.join([exe, __name__ + ".prog2", 'part1']), timeout=2)
        #test.logfile = sys.stdout
        #test.logfile = ArduinoBuilder.open_log('arduino.log', 'a')
        test.sendline("200")
        test.expect('.*period: 20\d')
        test.terminate()

    def test_serial_lower_limit(self) :
        exe = self.context['builder'].get_exe()
        test = pexpect.spawnu(' '.join([exe, __name__ + ".prog2", 'part1']), timeout=2)
        #test.logfile = sys.stdout
        #test.logfile = ArduinoBuilder.open_log('arduino.log', 'a')
        test.sendline("50")
        test.expect('.*period: 10\d')
        test.terminate()

    def test_serial_upper_limit(self) :
        exe = self.context['builder'].get_exe()
        test = pexpect.spawnu(' '.join([exe, __name__ + ".prog2", 'part1']), timeout=2)
        #test.logfile = sys.stdout
        #test.logfile = ArduinoBuilder.open_log('arduino.log', 'a')
        test.sendline("1200")
        test.expect('.*period: 100\d')
        test.terminate()


class Part2(unittest.TestCase) :

    def __init__(self, name, context, *args, **kwargs) :
        super().__init__(name, *args, **kwargs)
        self.context = context;

    def test_right_button(self) :
        exe = self.context['builder'].get_exe()
        test = pexpect.spawnu(' '.join([exe, __name__ + ".prog2", 'part2r']), timeout=2)
        #test.logfile = sys.stdout
        #test.logfile = ArduinoBuilder.open_log('arduino.log', 'a')
        test.expect('.*period: 100\d', timeout=30)
        test.expect('.*period: 100\d')
        test.terminate()

    def test_left_button(self) :
        exe = self.context['builder'].get_exe()
        test = pexpect.spawnu(' '.join([exe, __name__ + ".prog2", 'part2l']), timeout=2)
        #test.logfile = sys.stdout
        #test.logfile = ArduinoBuilder.open_log('arduino.log', 'a')
        test.expect('.*period: 10\d', timeout=30)
        test.expect('.*period: 10\d')
        test.terminate()

patterns = [
    ['pro(g|j)2_part1.ino', ArduinoBuilder, Part1],
    ['pro(g|j)2_part2.ino', ArduinoBuilder, Part1, Part2]
] 

files = [
    ['bin4counter.ino'],
    ['bin4io.ino'],
    ['if-else-if.ino'],
    ['button-counter.ino'],
    ['pro(g|j(ect)?)2_part1.ino', ArduinoBuilder, Part1],
    ['pro(g|j(ect)?)2_part2.ino', ArduinoBuilder, Part1, Part2],
]

name = "Project 2 Grader"
