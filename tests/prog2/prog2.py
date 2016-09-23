import sketch
from tests.fcounter import FrequencyCounter
from tests.button import Button
from emu.emulator import emu, Pin, PinMode

ledpin = 2
leftButton = 13
rightButton = 12

emu.pins[ledpin] = FrequencyCounter()
emu.pins[leftButton] = Button()
emu.pins[rightButton] = Button()

def test_run(part) :
    if part == 'part1' :
        return test_run_part1()
    elif part == 'part2l' :
        return test_run_part2l()
    elif part == 'part2r' :
        return test_run_part2r()
    else :
        print ("ERROR: No test chosen.")
        return False

# part 1
def test_run_part1() :    
    sketch.setup();
    while True :
        sketch.loop();
        counter = emu.pins[ledpin]
        period = counter.get_period() / 2
        if period > 0 :
            print ("period:", int(period))

# part 2
def test_run_part2l() :    
    sketch.setup();
    for i in range(100) :
        sketch.loop();
        if i % 2  == 0 :
            emu.pins[leftButton].push()

        counter = emu.pins[ledpin]
        period = counter.get_period() / 2
        if period > 0 :
            print ("period:", int(period))

# part 2
def test_run_part2r() :    
    sketch.setup();
    for i in range(100) :
        sketch.loop();
        if i % 2  == 0 :
            emu.pins[rightButton].push()
        
        counter = emu.pins[ledpin]
        period = counter.get_period() / 2
        if period > 0 :
            print ("period:", int(period))


