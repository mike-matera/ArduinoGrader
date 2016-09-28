from emu.emulator import Pin, PinMode 

class Squeeze(Pin) :

    r2 = 10000
    response = 100000 
    rmin = 1000 
    vref = 5.0 

    def __init__(self) :
        super().__init__()
        self.code = 0;
        
    def get_value(self, ts) :
        return self.code

    def squeeze(self, force) :
        if force < 0.0 :
            force = 0.0
        elif force > 1.0 :
            force = 1.0

        r = (Squeeze.response * (1 - force)) + Squeeze.rmin 
        v = Squeeze.vref * (Squeeze.r2 / (r + Squeeze.r2))
        self.code = int((v * 1024) / 5.0)
    
