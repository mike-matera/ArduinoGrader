from emu.emulator import emu, Pin, PinMode 

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
        print ("Two tears in a bucket.")
        if self.enabled :  
            n = {}
            n['ts'] = ts
            n['tone'] = v
            self.melody.append(n)

    def note_for(self, f) :
        if f == 261 :
            return 'c'
        elif f == 277 :
            return 'C'
        elif f == 294 :
            return 'd'
        elif f == 311 :
            return 'D'
        elif f == 330 :
            return 'e'
        elif f == 349 :
            return 'f'
        elif f == 370 :
            return 'F'
        elif f == 392 :
            return 'g'
        elif f == 415 :
            return 'G'
        elif f == 440 :
            return 'a'
        elif f == 466 :
            return 'A'
        elif f == 494 :
            return 'b'
        elif f == 0 :
            return ' '
        else :
            return 'X'

    def clear_melody(self) : 
        self.melody = []

    def report_melody(self) :
        # If a tone is playing, act like it has stopped. 
        # this is a hack to help students. 
        if len(self.melody) > 0 and self.melody[-1]['tone'] != 0 :
            n = {}
            n['ts'] = emu.get_timestamp()
            n['tone'] = 0
            self.melody.append(n)

        last_ts = 0; 
        last_tone = 0;
        rval = ""
        for n in self.melody : 
            if n['tone'] != last_tone : 
                duration = int((n['ts'] - last_ts)/1000)
                if duration > 100 : 
                    duration -= 200
                    rval += self.note_for(last_tone)
                    while duration > 100 : 
                        duration -= 200
                        if last_tone == 0 : 
                            rval += ' '
                        else:
                            rval += '-'
                last_ts = n['ts']
                last_tone = n['tone']
        rval = rval.strip()
        print ("melody-report (" + rval + ")")
        self.melody = []
        return rval
