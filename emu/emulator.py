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

    def set_mode(self, m) :
        self.mode = m

    def get_mode(self) :
        return self.mode; 

    def set_value(self, v) :
        self.value = v;

    def get_value(self) :
        return self.value
    
class Emulator :
    def __init__(self) :
        self.props = {}
        self.pins = []
        for p in range(NUMPINS) :
            self.pins.append(Pin())
    
    # Property interface for the emulator. 
    def get_property(self, **kwargs) :
        if kwargs['key'] in self.props :
            return self.props[kwargs['key']]
        else:
            return None

    def set_property(self, **kwargs) :
        self.props[kwargs['key']] = kwargs['value']    

    # Pin interface for the emulator.
    def set_pinmode(self, **kwargs) : 
        self.pins[int(kwargs['pin'])].set_mode(PinMode(int(kwargs['mode'])))

    def get_pinmode(self, **kwargs) :
        return self.pins[int(kwargs['pin'])].get_mode()

    def set_pinvalue(self, **kwargs) :
        self.pins[int(kwargs['pin'])].set_value(int(kwargs['value']))
    
    def get_pinvalue(self, **kwargs) :
        return self.pins[int(kwargs['pin'])].get_value()
        
emu = Emulator()
