
import sketch
from emu.emulator import emu, Pin, PinMode

# Watch pin 13 
class Blinker(Pin) :    
    def set_value(self, v) :
        if (self.mode == PinMode.kOutput) :
            if (v == 1) :
                print ('LED ON')
            else :
                print ('LED OFF')

emu.pins[13] = Blinker()

# Test entry point. 
def test_run() :    
    sketch.setup();
    for x in range(2) :
        sketch.loop();
