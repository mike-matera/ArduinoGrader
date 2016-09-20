

class Emulator :
    def __init__(self) :
        self.props = {}

    def get_property(self, **kwargs) :
        if kwargs['key'] in self.props :
            return self.props[kwargs['key']]
        else:
            return None

    def set_property(self, **kwargs) :
        self.props[kwargs['key']] = kwargs['value']    

    def set_pinmode(self, **kwargs) : 
        pass 

    def get_pinmode(self, **kwargs) :
        pass 

    def set_pinvalue(self, **kwargs) :
        pass
    
    def get_pinvalue(self, **kwargs) :
        pass 


emu = Emulator()
