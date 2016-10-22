import unittest
import subprocess 
import pexpect 
import sys
import re 

from tests.base import GraderBase 

class Part1(GraderBase) :

    def test_word_is_not_palindrome(self) :
        '''Testing a non-palindrome word'''
        with self.run_test(__name__ + ".prog5", 'word', timeout=1) as test :
            test.send('notaplaindrome\n')
            try:
                test.expect('(?i)is\s+not')
            except Exception :
                self.fail("Incorrect response to the word 'notaplaindrome'")

    def test_word_is_palindrome(self) :
        '''Testing a palindrome word'''
        with self.run_test(__name__ + ".prog5", 'word', timeout=1) as test :
            test.send('aviddiva\n')
            try:
                test.expect('(?i)is\s+a')
            except Exception :
                self.fail("Incorrect response to the word 'aviddiva'")

class Part2(GraderBase) :

    def test_sentence_is_palindrome(self) :
        '''Tesing a palindrome sentence.'''
        with self.run_test(__name__ + ".prog5", 'sentence', timeout=1) as test :
            test.send("amy must i jujitsu my ma\n")
            try:
                test.expect('(?i)palindrome\s+detected')
            except Exception :
                self.fail("Failed to detect a palindrome.")

    def test_abort(self) :
        '''Tesing the left button.'''
        with self.run_test( __name__ + ".prog5", 'sentence', timeout=1) as test :       
            test.send('blah blah blah blah blah blah blah blah blah blah blah')
            try:
                test.expect('(?i)abort')
            except Exception :
                self.fail("Failed to abort when I pushed the left button.")

files = [
    ['palindrome_word.ino', Part1],
    ['palindrome_sentence.ino', Part2],
]

name = "Project 5 Grader"
