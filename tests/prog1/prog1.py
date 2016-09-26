import sketch
from tests.fcounter import FrequencyCounter
from emu.emulator import emu, Pin, PinMode

emu.pins[13] = FrequencyCounter()
emu.pins[13].set_name("LED")

# Test entry point. 
def test_run() :    
    sketch.setup();
    for x in range(2) :
        sketch.loop();

    emu.pins[13].report()
