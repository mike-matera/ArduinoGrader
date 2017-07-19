import unittest
import subprocess 
import pexpect 
import sys
import re 

from tests.base import GraderBase 

class Part1(GraderBase) :

    def test_00_dominant_blue(self) :
        with self.run_test( __name__ + ".color1", timeout=5) as test :
            test.send("1 2 3\n")
            try :
                test.expect(['blue'])
            except Exception :
                self.fail("I didn't see any output from your program.")

files = [
    ['colorimeter.ino', Part1, ['Color.h', 'Color.cpp']],
]

name = "Colorimeter Part 1 Grader"
