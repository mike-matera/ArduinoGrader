import sketch
from tests.button import Button
from emu.emulator import emu, Pin, PinMode

leftButton = 12
emu.pins[leftButton] = Button()

def test_run(part) :
    if part == 'word' :
        return test_run_word()
    elif part == 'sentence' :
        return test_run_sentence()
    elif part == 'abort' :
        return test_run_abort()
    else :
        print ("ERROR: No test chosen.")
        return False

def test_run_word() :
    sketch.setup()
    sketch.loop()

def test_run_sentence() :
    sketch.setup()
    sketch.loop()

def test_run_abort() :
    sketch.setup()
    emu.pins[leftButton].push_delay(100)
    sketch.loop()

