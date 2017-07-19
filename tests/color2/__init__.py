import unittest
import subprocess 
import pexpect 
import sys
import re 

from tests.base import GraderBase 

class Part1(GraderBase) :

    def test_00_various_colors(self) :
        '''Test various colors'''
        with self.run_test( __name__ + ".color2", timeout=5) as test :
            try :
                test.expect(['(?i)dominant.*red'])
                test.expect(['(?i)dominant.*green'])
                test.expect(['(?i)dominant.* blue'])
            except Exception :
                self.fail("I didn't see the right output from your program.")

class Part1(GraderBase) :

    def test_00_various_colors(self) :
        '''Test various colors'''
        with self.run_test( __name__ + ".color2", timeout=5) as test :
            try :
                test.expect(['(?i)dominant.*red'])
                test.expect(['(?i)dominant.*green'])
                test.expect(['(?i)dominant.* blue'])
            except Exception :
                self.fail("I didn't see the right output from your program.")

files = [
    ['colorimeter(-part2)?.ino', Part1, ['Color.h', 'Color.cpp']],
    ['colorimeter-part2xc.ino', Part1, ['Color.h', 'Color.cpp']],
]

name = "Colorimeter Part 2 Grader"
