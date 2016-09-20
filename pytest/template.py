import emu
import sketch

# Test entry point. 

def test_run () :    
    sketch.setup();
    for x in range(2) :
        print ("Blink")
        sketch.loop();
