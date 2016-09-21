import sketch
from tests.fcounter import FrequencyCounter
from emu.emulator import emu, Pin, PinMode

ledpin = 2

emu.pins[ledpin] = FrequencyCounter()

# Test entry point. 
def test_run() :    
    sketch.setup();
    while True :
        sketch.loop();
        counter = emu.pins[ledpin]
        period = counter.get_period() / 2
        if period > 0 :
            print ("period:", int(period))
