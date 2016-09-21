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

# Test entry point. 
def test_run() :    
    sketch.setup();
    for i in range(50) :
        sketch.loop();
        if i % 2  == 0 :
            print ("+ Pushing left button for 0.1 seconds.")
            emu.pins[leftButton].push_for(0.1)

        counter = emu.pins[ledpin]
        period = counter.get_period() / 2
        if period > 0 :
            print ("period:", int(period))

    # for i in range(44) :
    #     sketch.loop();
    #     if i % 2  == 0 :
    #         print ("+ Pushing right button for 1 second.")
    #         emu.pins[rightButton].push_for(1)

    #     counter = emu.pins[ledpin]
    #     period = counter.get_period() / 2
    #     if period > 0 :
    #         print ("period:", int(period))

