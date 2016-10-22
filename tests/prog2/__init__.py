import unittest
import subprocess 
import pexpect 
import sys

from tests.base import GraderBase 

class Part1(GraderBase) :

    def test_does_blink(self) :
        '''Testing that your program blinks with no input.'''
        with self.run_test(__name__ + ".prog2", 'part1', timeout=2) as test :
            try:
                test.expect('.*period: (\d+)')
            except pexpect.TIMEOUT : 
                self.fail("I can't see the LED blinking.")

    def test_serial_valid_number(self) :
        '''Testing that your program accepts a valid input.'''
        with self.run_test(__name__ + ".prog2", 'part1', timeout=2) as test :
            test.send("200\n")
            try:
                test.expect('.*period: (\d+)')
                test.expect('.*period: (\d+)')
                test.expect('.*period: (\d+)')
            except pexpect.TIMEOUT : 
                self.fail("I can't see the LED blinking.")
                self.assertEqual(200, int(test.match.group(1)), "The delay isn't 200 like I specified.")

    def test_serial_lower_limit(self) :
        '''Testing that your program rejects a low number.'''
        with self.run_test(__name__ + ".prog2", 'part1', timeout=2) as test :
            test.send("50\n")
            try:
                test.expect('.*period: (\d+)')
                test.expect('.*period: (\d+)')
            except pexpect.TIMEOUT : 
                self.fail("I can't see the LED blinking.")
                self.assertEqual(100, int(test.match.group(1)), "The delay isn't 100 after entering a period of 50")

    def test_serial_upper_limit(self) :
        '''Testing that your program rejects a high number.'''
        with self.run_test(__name__ + ".prog2", 'part1', timeout=2) as test :
            test.send("1200\n")
            try:
                test.expect('.*period: (\d+)')
                test.expect('.*period: (\d+)')
            except pexpect.TIMEOUT : 
                self.fail("I can't see the LED blinking.")
                self.assertEqual(1000, int(test.match.group(1)), "The delay isn't 1000 after entering a period of 1200")


class Part2(GraderBase) :

    def test_right_button(self) :
        '''Testing the right button.'''
        with self.run_test(__name__ + ".prog2", 'part2r', timeout=2) as test :
            test.expect('.*period: (\d+)')
            test.expect('.*period: (\d+)')
            start = test.match.group(1)
            test.expect('.*period: (\d+)')
            test.expect('.*period: (\d+)')
            end = test.match.group(1)
            self.assertTrue(end > start, msg="The right button doesn't seem to make the blink delay larger.")
    
            test.expect('.*period: (\d+)')
            period = int(test.match.group(1))
            count = 0
            while period < 1000 :
                test.expect('.*period: (\d+)')
                period = int(test.match.group(1))
                count += 1
                if count == 100 :
                    self.fail("Pushing the right button never got to the maximum period.")

            test.expect('.*period: (\d+)')
            period = int(test.match.group(1))
            if (period - 1000) < 5 :
                period = 1000

            self.assertEqual(period, 1000, msg="The period exceeded the maximum while pushing the button.")

    def test_left_button(self) :
        '''Testing the left button.'''
        with self.run_test( __name__ + ".prog2", 'part2l', timeout=2) as test :
            test.expect('.*period: (\d+)')
            test.expect('.*period: (\d+)')
            start = test.match.group(1)
            test.expect('.*period: (\d+)')
            test.expect('.*period: (\d+)')
            end = test.match.group(1)
            self.assertTrue(end < start, msg="The left button doesn't seem to make the blink delay smaller.")

            test.expect('.*period: (\d+)')
            period = int(test.match.group(1))
            count = 0
            while period > 100 :
                test.expect('.*period: (\d+)')
                period = int(test.match.group(1))
                count += 1
                if count == 100 :
                    self.fail("Pushing the left button never got to the minimum period.")

            test.expect('.*period: (\d+)')
            period = int(test.match.group(1))
            if (period - 100) < 5 :
                period = 100

            self.assertEqual(period, 100, msg="The period exceeded the minimum while pushing the button.")

files = [
    ['pro(g|j(ect)?)2_part1.ino', Part1],
    ['pro(g|j(ect)?)2_part2.ino', Part1, Part2],
]

name = "Project 2 Grader"
