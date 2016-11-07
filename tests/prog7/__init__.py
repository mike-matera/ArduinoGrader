import unittest
import subprocess 
import pexpect 
import sys
import re 

from tests.base import GraderBase 

class Part1(GraderBase) :

    def do_notes(self, melody) : 
        with self.run_test( __name__ + ".prog7", 'part1', timeout=5) as test :
            test.send(melody + "\n")
            try :
                test.logfile = sys.stdout
                test.expect(['melody-report \(([^\)]*)\)'])
                seen = test.match.group(1)
                self.assertEqual(melody, seen, "Your program didn't seem to play the melody: " + melody + " received: " + seen)
                    
            except Exception :
                self.fail("Your sketch didn't seem to produce any sound.")

    def test_00_scale(self) :
        self.do_notes('cCdDefFgGaAb')

    def test_01_mary_had_a_little_lamb(self) :
        self.do_notes('agfgaaa ggg aaa agfgaaaaggagf')

    def test_02_yankee_doodle_dandy(self) :
        self.do_notes('ffgafag ffgaf-e- ffgaAagfecdef-f-')

    def test_03_ode_to_joy(self): 
        self.do_notes('e-e-f-g-g-f-e-d-c-c-d-e-e--dd--  e-e-f-g-g-f-e-d-c-c-d-e-d--cc--  d-d-e-c-d-efe-c-d-efe-d-c-d-c-  e-e-f-g-g-f-e-d-c-c-d-e-d--cc--')

class Part2(GraderBase) :

    def test_00_extra_credit(self) :
        melody1 = 'agfgaaa ggg aaa agfgaaaaggagf'
        melody2 = 'ffgafag ffgaf-e- ffgaAagfecdef-f-'
        melody3 = 'e-e-f-g-g-f-e-d-c-c-d-e-e--dd-- e-e-f-g-g-f-e-d-c-c-d-e-d--cc-- d-d-e-c-d-efe-c-d-efe-d-c-d-c- e-e-f-g-g-f-e-d-c-c-d-e-d--cc--'
        melody4 = 'c-------g-------c-------g-------c-------g-------c-------g---c---g-------g-------g-------g-------c-------g-------c-------g---c---'
        with self.run_test( __name__ + ".prog7", 'part2', timeout=5) as test :
            test.send(melody1 + "\n")
            test.send(melody2 + "\n")
            test.send(melody3 + "\n")
            test.send(melody4 + "\n")
            try:
                test.expect(['melody-report \(([^\)]*)\)'])
                self.assertEqual(melody2, test.match.group(1), "You didn't play the first melody correctly.")
            except Exception as e : 
                print (e)
                self.fail("I never saw any output from your program.")

            try :
                test.expect(['melody-report \(([^\)]*)\)'])
                self.assertEqual(melody3, test.match.group(1), "You didn't play the second melody correctly.")
                test.expect(['melody-report \(([^\)]*)\)'])
                self.assertEqual(melody4, test.match.group(1), "You didn't play the third melody correctly.")
                test.expect(['melody-report \(([^\)]*)\)'])
                self.assertEqual(melody1, test.match.group(1), "You didn't play the fourth melody correctly.")
            except Exception : 
                self.fail("You didn't play all four melodies.")


files = [
    ['(?i)chiptunes?\.ino', Part1],
    ['(?i)chiptunes?-multi\.ino', Part2],
]

name = "Project 7 Grader"
