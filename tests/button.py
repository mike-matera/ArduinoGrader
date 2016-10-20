
from emu.emulator import Pin, PinMode 

class Button(Pin) :
    def __init__(self) :
        super().__init__()
        self.armed = 0;
        
    def get_value(self, ts) :
        if self.mode == PinMode.kPullup : 
            if self.armed > 0 :
                self.armed -= 1
                if self.armed == 0 :
                    return 0
                else:
                    return 1
            else : 
                return 1
        else:
            return self.value

    def push(self) :
        self.armed = 1

    def push_delay(self, delay) :
        self.armed = delay
