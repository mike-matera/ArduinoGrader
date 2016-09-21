import subprocess 
import pexpect 
import sys

def do_part1(exe) :
    try:
        test = pexpect.spawnu(' '.join([exe, __name__ + ".prog1"]))
        test.logfile = sys.stdout
        test.expect('.*frequency OK')
    except Exception as e: 
        print (str(e))
        return False 
    return True

def do_part2(exe) :
    try:
        test = pexpect.spawnu(' '.join([exe, __name__ + ".prog1"]))
        test.logfile = sys.stdout
        test.expect('.*200')
        test.expect('.*400')
        test.expect('.*frequency OK')
    except Exception as e: 
        print (str(e))
        return False 

    return True

patterns = [
    ['prog1_part1.ino', do_part1, 'Part 1'],
    ['prog1_part2.ino', do_part2, 'Part 2']
] 

name = "Project 1 Grader"
