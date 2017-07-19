import sketch
from emu.emulator import emu, Pin, PinMode
from tests.analog import Analog 

photoPin = 14
emu.pins[photoPin] = Analog()

def test_run() :
    emu.pins[photoPin].set_values([20, 10, 100, 900, 700, 200, 30])
    sketch.setup();
    sketch.loop();
