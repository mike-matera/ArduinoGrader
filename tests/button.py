
import time 

from emu.emulator import Pin, PinMode 

class Button(Pin) :
    def __init__(self) :
        super().__init__()
        self.mark = 0;

    def get_value(self, v) :
        if self.mode == PinMode.kInput : 
            if time.time < mark :
                return 0
            else:
                return 1
        else:
            return self.value

    def push_for(self, pushtime) :
        self.mark = time.time + pushtime
