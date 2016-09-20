

class Emulator :
    def __init__(self) :
        self.props = {}

    def get_property(self, **kwargs) :
        for key, value in kwargs.items() : 
            print ("key:", key, "value:", value)
        return self.props[name]

    def set_property(self, name, value) :
        self.props[name] = value    

emu = Emulator()
