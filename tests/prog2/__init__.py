import subprocess 
import pexpect 
import sys

def do_part1(exe) :    
    try:
        test = pexpect.spawnu(' '.join([exe, __name__ + ".prog2", 'part1']))
        test.logfile = sys.stdout
        print ("+ Testing if your sketch responds to serial input.")
        test.expect('.*period: (\d+)')
        test.sendline("200")
        test.expect('.*period: 20\d')
        print ("+ Testing a low value.")
        test.sendline("50")
        test.expect('.*period: 10\d')
        print ("+ Testing a high value.")
        test.sendline("1200")
        test.expect('.*period: 100\d')
        print ("++ Pass")
        test.terminate()

    except Exception as e: 
        print (str(e))
        return False 
    return True

def do_part2(exe) :
    print ("+ Testing if part2 meets the part1 specification")
    do_part1(exe)
    try:
        test = pexpect.spawnu(' '.join([exe, __name__ + ".prog2", 'part2r']))
        test.logfile = sys.stdout
        print ("+ Testing the left button.")
        test.expect('.*period: 100\d', timeout=120)
        test.expect('.*period: 100\d')
        print ("+ pass: got to 1000")
        test.terminate()

        test = pexpect.spawnu(' '.join([exe, __name__ + ".prog2", 'part2l']))
        test.logfile = sys.stdout
        print ("+ Testing the right button.")
        test.expect('.*period: 10\d', timeout=120)
        test.expect('.*period: 10\d')
        print ("+ pass: got to 1000")
        test.terminate()

    except Exception as e: 
        print (str(e))
        return False 

    return True

patterns = [
    ['prog2_part1.ino', do_part1, 'Variable delay serial entry.'],
    ['prog2_part2.ino', do_part2, 'Variable delay serial and buttons.']
] 

name = "Project 2 Grader"
