import emu
import avr 

# Test lifecycle functions. 

def test_run () :    
    print ("About to setup()!")
    avr.setup();
    print ("About to loop()!")
    avr.loop();
    print ("About to exit")


#def test_setup() :
#    print ("Call to setup()")
#
#def test_loop(cnt) :
#    print ("Call to loop(): #", str(cnt))
#    return False
#
#def test_exit() :
#    print ("Exiting!")
