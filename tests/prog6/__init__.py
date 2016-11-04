import unittest
import subprocess 
import pexpect 
import sys
import re 

from tests.base import GraderBase 

class MonthNames(GraderBase) :

    def test_months(self) :
        '''Entering numbers 1 through 12'''
        with self.run_test(__name__ + ".prog6", timeout=1) as test :
            test.send('1\n')
            test.send('2\n')
            test.send('3\n')
            test.send('4\n')
            test.send('5\n')
            test.send('6\n')
            test.send('7\n')
            test.send('8\n')
            test.send('9\n')
            test.send('10\n')
            test.send('11\n')
            test.send('12\n')
            try:
                test.expect('(?i)january')
                test.expect('(?i)february')
                test.expect('(?i)march')
                test.expect('(?i)april')
                test.expect('(?i)may')
                test.expect('(?i)june')
                test.expect('(?i)july')
                test.expect('(?i)august')
                test.expect('(?i)september')
                test.expect('(?i)october')
                test.expect('(?i)november')
                test.expect('(?i)december')
            except Exception :
                self.fail("Garbled month!")

class OhmsLaw(GraderBase) :

    def test_E(self) :
        '''Testing volts calculator.'''
        with self.run_test(__name__ + ".prog6", timeout=1) as test :
            test.send("1\n")
            test.send("2\n")
            test.send("30\n")
            try:
                test.expect('60')
            except Exception :
                self.fail("Failed to compute V = I * E")

    def test_I(self) :
        '''Testing amps calculator.'''
        with self.run_test(__name__ + ".prog6", timeout=1) as test :
            test.send("2\n")
            test.send("120\n")
            test.send("12\n")
            try:
                test.expect('10')
            except Exception :
                self.fail("Failed to compute V = I * E")

    def test_R(self) :
        '''Testing ohms calculator.'''
        with self.run_test(__name__ + ".prog6", timeout=1) as test :
            test.send("3\n")
            test.send("130\n")
            test.send("13\n")
            try:
                test.expect('10')
            except Exception :
                self.fail("Failed to compute V = I * E")


files = [
    ['month_names.ino', MonthNames],
    ['ohms_law.ino', OhmsLaw],
]

name = "Project 6 Grader"
