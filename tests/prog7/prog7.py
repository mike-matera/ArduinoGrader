import sketch
from tests.button import Button
from emu.emulator import emu, Pin, PinMode
from tests.chiptune import ChipTune 

speakerPin = 3
leftButton = 12
rightButton = 13
emu.pins[speakerPin] = ChipTune()
emu.pins[leftButton] = Button()
emu.pins[rightButton] = Button()

def test_run(mode) :
    sketch.setup();
    emu.pins[speakerPin].clear_melody()
    if mode == 'part1' :
        while True: 
            sketch.loop();
            emu.pins[speakerPin].report_melody()
    else:
        turn = 0
        while True: 
            emu.pins[leftButton].push()
            sketch.loop();
            emu.pins[rightButton].push()
            sketch.loop();
            emu.pins[speakerPin].report_melody()
