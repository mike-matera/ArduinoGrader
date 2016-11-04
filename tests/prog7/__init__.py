import unittest
import subprocess 
import pexpect 
import sys
import re 

from tests.base import GraderBase 

class Part1(GraderBase) :

    def test_scale(self) : 
        with self.run_test( __name__ + ".prog7", timeout=1) as test :
            test.logfile = sys.stdout
            scale = 'cCdDefFgGaAb'
            test.send(scale + "\n")
            try :
                test.expect(['DEBUG'])
            except Exception :
                self.fail("Error.")                


files = [
    ['chiptunes?.ino', Part1],
]

name = "Project 7 Grader"
