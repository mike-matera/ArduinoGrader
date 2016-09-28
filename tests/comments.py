import sys 
import os
import re 
import tempfile 
import subprocess 
import multiprocessing 
import shutil 
import glob 
import itertools 
import unittest
import zipfile 


class Comments(unittest.TestCase) : 

    def __init__(self, name, context, *args, **kwargs) :
        super().__init__(name, *args, **kwargs)
        self.context = context

    def test_has_block_comment(self) :
        with open(self.context['program']) as sketch :
            data = sketch.read()

        m = re.search('(Author|By)\s*:\s+([^\n]+)', data)
        assert(m is not None)

        m = re.search('Date\s*:\s+([^\n]+)', data)
        assert(m is not None)
