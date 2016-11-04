
from emu.emulator import Pin, PinMode 

class ChipTune(Pin) :

    def __init__(self) :
        super().__init__()
        self.enabled = False;
        self.melody = []

    def set_mode(self, ts, m) :
        super().set_mode(ts, m)
        if m == PinMode.kSound :
            self.enabled = True

    def set_value(self, ts, v) :
        super().set_value(ts, v)
        if self.enabled :  
            n = {}
            n['ts'] = ts
            n['tone'] = v
            self.melody.append(n)

    def get_melody(self) :
        for i in range(0, len(self.melody)) :
            print(i, ":", self.melody[i])
