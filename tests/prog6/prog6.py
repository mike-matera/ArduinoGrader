import sketch
from tests.button import Button
from emu.emulator import emu, Pin, PinMode

leftButton = 12
emu.pins[leftButton] = Button()

def test_run() :
    sketch.setup()
    while True:
        sketch.loop()
