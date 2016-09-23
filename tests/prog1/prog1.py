import sketch
from tests.fcounter import FrequencyCounter
from emu.emulator import emu, Pin, PinMode

emu.pins[13] = FrequencyCounter()

# Test entry point. 
def test_run() :    
    sketch.setup();
    for x in range(2) :
        sketch.loop();

    counter = emu.pins[13]
    print ("period:", counter.get_period())

