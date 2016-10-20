import unittest
import subprocess 
import pexpect 
import sys
import re 

class Part1(unittest.TestCase) :

    def __init__(self, name, context, *args, **kwargs) :
        super().__init__(name, *args, **kwargs)
        self.context = context;

    def send_stuff(self, stuff) :
        exe = self.context['builder'].get_exe()
        test = pexpect.spawnu(' '.join([exe, __name__ + ".prog5", 'word']), timeout=1)
        #test.logfile = sys.stdout
        test.send(stuff + "\n")
        return test

    def test_word_is_not_palindrome(self) :
        '''Testing a non-palindrome word'''
        test = self.send_stuff('notaplaindrome')
        try:
            test.expect('(?i)is\s+not')
        except Exception :
            self.fail("Incorrect response to the word 'notaplaindrome'")

    def test_word_is_palindrome(self) :
        '''Testing a palindrome word'''
        test = self.send_stuff('aviddiva')
        try:
            test.expect('(?i)is\s+a')
        except Exception :
            self.fail("Incorrect response to the word 'aviddiva'")

class Part2(unittest.TestCase) :

    def __init__(self, name, context, *args, **kwargs) :
        super().__init__(name, *args, **kwargs)
        self.context = context;

    def send_stuff(self, stuff, arg) :
        exe = self.context['builder'].get_exe()
        test = pexpect.spawnu(' '.join([exe, __name__ + ".prog5", arg]), timeout=1)
        #test.logfile = sys.stdout
        test.send(stuff + "\n")
        return test

    def test_sentence_is_palindrome(self) :
        '''Tesing a palindrome sentence.'''
        #test = self.send_stuff('amy must i jujitsu my ma')
        test = self.send_stuff("amy must i jujitsu my ma", 'sentence')
        try:
            test.expect('(?i)palindrome\s+detected')
        except Exception :
            self.fail("Failed to detect a palindrome.")

    def test_abort(self) :
        '''Tesing the left button.'''
        test = self.send_stuff('blah blah blah blah blah blah blah blah blah blah blah', 'abort')
        try:
            test.expect('(?i)abort')
        except Exception :
            self.fail("Failed to abort when I pushed the left button.")

files = [
    ['palindrome_word.ino', Part1],
    ['palindrome_sentence.ino', Part2],
]

name = "Project 5 Grader"
