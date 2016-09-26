import time 

from enum import Enum 

NUMPINS   = 20
NUMANPINS = 8

class PinMode(Enum):
    kOutput   = 0 
    kInput    = 1 
    kPullup   = 2 
    kPWM      = 3 
    kSound    = 4 
    kAnalog   = 5

class Pin :
    def __init__(self) :
        self.name = ""
        self.mode = PinMode.kPullup;
        self.value = 0; 

    def set_mode(self, ts, m) :
        self.mode = m

    def get_mode(self, ts) :
        return self.mode; 

    def set_value(self, ts, v) :
        self.value = v;

    def get_value(self, ts) :
        return self.value

    def set_name(self, n) :
        self.name = n
    
    def get_name(self) :
        return self.name

class Emulator :
    def __init__(self) :
        self.props = {}
        self.pins = []
        for p in range(NUMPINS) :
            pin = Pin()
            pin.set_name(str(p))
            self.pins.append(Pin())
    
    # Property interface for the emulator. 
    def get_property(self, ts, **kwargs) :
        if kwargs['key'] in self.props :
            return self.props[kwargs['key']]
        else:
            return None

    def set_property(self, ts, **kwargs) :
        self.props[kwargs['key']] = kwargs['value']    

    # Pin interface for the emulator.
    def set_pinmode(self, ts, **kwargs) : 
        self.pins[int(kwargs['pin'])].set_mode(ts, PinMode(int(kwargs['mode'])))

    def get_pinmode(self, ts, **kwargs) :
        return self.pins[int(kwargs['pin'])].get_mode(ts)

    def set_pinvalue(self, ts, **kwargs) :
        self.pins[int(kwargs['pin'])].set_value(ts, int(kwargs['value']))
    
    def get_pinvalue(self, ts, **kwargs) :
        return self.pins[int(kwargs['pin'])].get_value(ts)

emu = Emulator()
