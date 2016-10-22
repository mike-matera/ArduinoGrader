import unittest
import subprocess 
import pexpect 
import sys
import re 

from tests.base import GraderBase 

class Part1(GraderBase) :

    def get_code(self, color) :
        if color == 'B' : 
            return 0
        elif color == 'Br' or color == 'BR' :
            return 1 
        elif color == 'R' :
            return 2
        elif color == 'O' :
            return 3
        elif color == 'Y' :
            return 4
        elif color == 'G' :
            return 5
        elif color == 'Bu' or color == 'BU' :
            return 6
        elif color == 'V' :
            return 7
        elif color == 'Gy' or color == 'GY' :
            return 8
        elif color == 'W' :
            return 9
        assert(False)

    def get_resistor(self, code) :
        b = code.split()
        base = self.get_code(b[0]) * 10 + self.get_code(b[1])
        return base * pow(10, self.get_code(b[2]))

    def find_ohms(self, code) :
        with self.run_test( __name__ + ".prog4", timeout=1) as test :
            test.send(code + "\n")

            try :
                test.expect(['(?i)(\d+)\s*(m|k)?\s*ohms?'])
            except Exception :
                self.fail("I didn't see a value that I understand.")
                
            num = int(test.match.group(1))
            if test.match.group(2) is None :
                self.assertLess(num, 1000, msg='Ohms values cannot exceed 1000, they should be k ohms.')
            elif test.match.group(2) == 'k' or test.match.group(2) == 'K' :
                self.assertLess(num, 1000, msg='K Ohms values cannot exceed 1000, they should be M ohms.')
                num *= 1000
            elif test.match.group(2) == 'm' or test.match.group(2) == 'M' :
                num *= 1000000
            else :
                assert (False)

            self.assertEqual(num, self.get_resistor(code), 'The program returned the wrong value for the code: ' + code)

    def test_100(self) :
        '''Testing 100 ohms'''
        self.find_ohms("Br B Br")

    def test_1k(self) :
        '''Testing 1k ohms'''
        self.find_ohms("Br B R")

    def test_1M(self) :
        '''Testing 1M ohms'''
        self.find_ohms("Br B G")

    def test_10M(self) :
        '''Testing 10M ohms'''
        self.find_ohms("Br B B")

    def test_all_colors(self) :
        '''Testing various colors'''
        self.find_ohms("Br B Br")
        self.find_ohms("BR B Br")
        self.find_ohms("R B Br")
        self.find_ohms("O B Br")
        self.find_ohms("Y B Br")
        self.find_ohms("G B Br")
        self.find_ohms("Bu B Br")
        self.find_ohms("BU B Br")
        self.find_ohms("V B Br")
        self.find_ohms("Gy B Br")
        self.find_ohms("GY B Br")
        self.find_ohms("W B Br")

    def test_w_w_w(self) :
        '''(Extra Credit) Testing W W W'''
        self.find_ohms("W W W")

files = [
    ['resistor_codes?.ino', Part1],
]

name = "Project 4 Grader"
