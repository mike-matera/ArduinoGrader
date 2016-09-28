import sketch
from tests.squeeze import Squeeze
from emu.emulator import emu, Pin, PinMode

sensorPin = 15

s = Squeeze()
emu.pins[sensorPin] = s

def test_run() :
    sketch.setup();
    s.squeeze(0.75)
    while True :
        sketch.loop();
