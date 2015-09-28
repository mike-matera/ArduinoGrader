#include <iostream>

#include "emulator.h"

using std::cout; 
using std::endl;
using std::string; 

static int leftLed = -1; 
static int rightLed = -1; 

bool sawLeft = false;
bool sawRight = false; 

void test_pinchange(int pin, const PinState &prev, const PinState &next) {
  if (prev == next)
    return;

  if (!prev.is_enabled() && next.is_enabled()) {
    cout << "Pin: " << pin << " has been enabled." << endl; 
    if (leftLed == -1) {
      leftLed = pin; 
    }else{
      rightLed = pin; 
    }
  }

  if (Arduino.get_pin(leftLed).is_high() && Arduino.get_pin(rightLed).is_low()) {
    cout << "Left" << endl;
    sawLeft = true;
  }

  if (Arduino.get_pin(leftLed).is_low() && Arduino.get_pin(rightLed).is_high()) {
    cout << "Right" << endl;
    sawRight = true;
  }
  
}

bool test_loop(int count) {
  cout << "loop #" << count << endl;
  return (count < 4 && !(sawLeft && sawRight));
}

void test_exit(void) {
  if (sawLeft && sawRight) {
    cout << "TEST PASSED" << endl;
  }else{
    cout << "TEST FAILED" << endl;
  }
}
