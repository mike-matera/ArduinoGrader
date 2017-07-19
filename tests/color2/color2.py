import sketch
from tests.button import Button
from emu.emulator import emu, Pin, PinMode

class PWM_LED(Pin) : 
    
    def __init__(self) :
        super().__init__()
        self.value = 0

    def set_value(self, ts, v) : 
        super().set_value(ts, v)
        self.value = (255 - v) / 255.0

    def led_value(self) :
        return self.value

leftButton = 12
rightButton = 13
emu.pins[leftButton] = Button()
emu.pins[rightButton] = Button()

redLED = 11
greenLED = 10
blueLED = 9
emu.pins[redLED] = PWM_LED()
emu.pins[greenLED] = PWM_LED()
emu.pins[blueLED] = PWM_LED()

class LightReflection(Pin) :
    def __init__(self) :
        super().__init__()
        self.red = 1.0
        self.green = 1.0
        self.blue = 1.0

    def set_object_color(self, r, g, b) :
        self.red = r 
        self.green = g
        self.blue = b
        
    def get_value(self, ts) :
        r = int(emu.pins[redLED].led_value() * self.red * 1023)
        g = int(emu.pins[greenLED].led_value() * self.green * 1023)
        b = int(emu.pins[blueLED].led_value() * self.blue * 1023)
        print ("Emulator: measured color is: (", r, g, b, ") sum:", r + g + b)
        return r + g + b

lightSensor = 14
emu.pins[lightSensor] = LightReflection()


def test_run() :
    sketch.setup()

    emu.pins[lightSensor].set_object_color(0.9, 0.5, 0.3)
    emu.pins[leftButton].push_delay(1)
    emu.pins[rightButton].push_delay(1)
    sketch.loop()
    sketch.loop()
    
    emu.pins[lightSensor].set_object_color(0.5, 0.9, 0.3)
    emu.pins[leftButton].push_delay(1)
    emu.pins[rightButton].push_delay(1)
    sketch.loop()
    sketch.loop()

    emu.pins[lightSensor].set_object_color(0.3, 0.5, 0.9)
    emu.pins[leftButton].push_delay(1)
    emu.pins[rightButton].push_delay(1)
    sketch.loop()
    sketch.loop()

    emu.pins[lightSensor].set_object_color(0.3, 0.5, 0.3)
    emu.pins[leftButton].push_delay(1)
    emu.pins[rightButton].push_delay(1)
    sketch.loop()
    sketch.loop()
        
