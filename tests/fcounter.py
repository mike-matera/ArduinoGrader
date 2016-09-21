
import time 

from emu.emulator import Pin, PinMode 

class FrequencyCounter(Pin) :
    def __init__(self) :
        super().__init__()
        self.enabled = False;
        self.mark = time.time()
        self.last = 0
        self.lowtime = 0
        self.hightime = 0

    def set_mode(self, m) :
        super().set_mode(m)
        if m == PinMode.kOutput :
            self.enabled = True
            self.mark = time.time()

    def set_value(self, v) :
        super().set_value(v)
        if self.enabled : 
            if self.last != v : 
                ntime = time.time()
                diff = int((ntime - self.mark) * 1000)
                self.mark = ntime
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
