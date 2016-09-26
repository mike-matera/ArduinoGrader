
from emu.emulator import Pin, PinMode 

class FrequencyCounter(Pin) :
    def __init__(self) :
        super().__init__()
        self.enabled = False;
        self.mark = 0
        self.last = 0
        self.lowtime = 0
        self.hightime = 0

    def set_mode(self, ts, m) :
        super().set_mode(ts, m)
        if m == PinMode.kOutput :
            self.enabled = True
            self.mark = ts

    def set_value(self, ts, v) :
        super().set_value(ts, v)
        if self.enabled : 
            if self.last != v : 
                diff = int((ts - self.mark) / 1000)
                self.mark = ts
                if self.last == 0 :
                    self.lowtime = diff
                else:
                    self.hightime = diff
                self.last = v

    def get_period(self) :
        if self.lowtime == 0 or self.hightime == 0 :
            return 0
        else:
            return self.lowtime + self.hightime 

    def get_low(self) :
        return self.lowtime 

    def get_high(self) :
        return self.hightime

    def report(self) : 
        print (self.name, "fc ( h:", self.hightime, "l:", self.lowtime, ")")
