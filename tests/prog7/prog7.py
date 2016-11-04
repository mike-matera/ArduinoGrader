import sketch
from tests.squeeze import Squeeze
from emu.emulator import emu, Pin, PinMode
from tests.chiptune import ChipTune 

speakerPin = 3
emu.pins[speakerPin] = ChipTune()

def test_run() :
    sketch.setup();
    while True: 
        sketch.loop();
        emu.pins[speakerPin].get_melody()
