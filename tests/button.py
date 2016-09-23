
from emu.emulator import Pin, PinMode 

class Button(Pin) :
    def __init__(self) :
        super().__init__()
        self.armed = False;
        
    def get_value(self, ts) :
        if self.mode == PinMode.kPullup : 
            if self.armed :
                self.armed = False
                return 0
            else : 
                return 1
        else:
            return self.value

    def push(self) :
        self.armed = True
