import unittest
import subprocess 
import pexpect 
import sys
import re 

from tests.base import GraderBase 

class Part1(GraderBase) :

    def test_00_does_sort(self) :

        with self.run_test( __name__ + ".prog8", timeout=1) as test :
            test.send("21\n")
            try:
                res = test.expect(["min(imum)?\s+(sample\s+)?is:\s+10", "Caught a signal: (\d+)"])
                if res == 1 :
                    self.fail("Your program caused a memory violation. This probably means that you exceeded your vector bounds.")
            except Exception as e : 
                self.fail("I didn't see you print the minimum sample.")

            try:
                test.expect(["max(imum)?\s+(sample\s+)?is:\s+900"])
            except Exception as e : 
                self.fail("I didn't see you print the maximum sample.")

            try:
                test.expect(["median\s+(sample\s+)?is:\s+100"])
            except Exception as e : 
                self.fail("I didn't see you print the median sample.")

files = [
    ['(?i)calibrate_?\.ino', Part1],
]

name = "Project 8 Grader"
