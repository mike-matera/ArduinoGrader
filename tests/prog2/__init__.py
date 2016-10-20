import unittest
import subprocess 
import pexpect 
import sys

class Part1(unittest.TestCase) :

    def __init__(self, name, context, *args, **kwargs) :
        super().__init__(name, *args, **kwargs)
        self.context = context;

    def test_does_blink(self) :
        '''Testing that your program blinks.'''
        exe = self.context['builder'].get_exe()
        test = pexpect.spawnu(' '.join([exe, __name__ + ".prog2", 'part1']), timeout=2)
        test.expect('.*period: (\d+)')
        test.terminate()

    def test_serial_valid_number(self) :
        '''Testing that your program accepts a valid input.'''
        exe = self.context['builder'].get_exe()
        test = pexpect.spawnu(' '.join([exe, __name__ + ".prog2", 'part1']), timeout=2, ignore_sighup=True)
        test.send("200\n")
        test.expect('.*period: (\d+)')
        test.expect('.*period: (\d+)')
        test.expect('.*period: (\d+)')
        self.assertEqual(200, int(test.match.group(1)), "The delay isn't 200 like I specified.")

    def test_serial_lower_limit(self) :
        '''Testing that your program rejects a low number.'''
        exe = self.context['builder'].get_exe()
        test = pexpect.spawnu(' '.join([exe, __name__ + ".prog2", 'part1']), timeout=2)
        test.send("50\n")
        test.expect('.*period: (\d+)')
        test.expect('.*period: (\d+)')
        self.assertEqual(100, int(test.match.group(1)), "The delay isn't 100 after entering a period of 50")

    def test_serial_upper_limit(self) :
        '''Testing that your program rejects a high number.'''
        exe = self.context['builder'].get_exe()
        test = pexpect.spawnu(' '.join([exe, __name__ + ".prog2", 'part1']), timeout=2)
        test.send("1200\n")
        test.expect('.*period: (\d+)')
        test.expect('.*period: (\d+)')
        self.assertEqual(1000, int(test.match.group(1)), "The delay isn't 1000 after entering a period of 1200")


class Part2(unittest.TestCase) :

    def __init__(self, name, context, *args, **kwargs) :
        super().__init__(name, *args, **kwargs)
        self.context = context;

    def test_right_button(self) :
        '''Testing the right button.'''
        exe = self.context['builder'].get_exe()
        test = pexpect.spawnu(' '.join([exe, __name__ + ".prog2", 'part2r']), timeout=2)
        try:
            test.expect('.*period: 100\d', timeout=30)
            test.expect('.*period: 100\d')
        except pexpect.TIMEOUT : 
            self.fail("The period never seemed to get to the maximum.")

    def test_left_button(self) :
        '''Testing the left button.'''
        exe = self.context['builder'].get_exe()
        test = pexpect.spawnu(' '.join([exe, __name__ + ".prog2", 'part2l']), timeout=2)
        try:
            test.expect('.*period: 10\d\s+', timeout=30)
            test.expect('.*period: 10\d\s+ ')
        except pexpect.TIMEOUT : 
            self.fail("The period never seemed to get to the minimum.")

files = [
    ['pro(g|j(ect)?)2_part1.ino', Part1],
    ['pro(g|j(ect)?)2_part2.ino', Part1, Part2],
]

name = "Project 2 Grader"
