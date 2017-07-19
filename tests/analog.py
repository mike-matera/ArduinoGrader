from emu.emulator import Pin, PinMode 

class Analog(Pin) :

    def __init__(self) :
        super().__init__()
        self.pos = 0;
        self.values = []

    def get_value(self, ts) :
        if len(self.values) == 0 :
            return 0;
        val = self.values[self.pos];
        self.pos = (self.pos + 1) % len(self.values)
        return val

    def set_values(self, values) :
        self.values = values; 
    
