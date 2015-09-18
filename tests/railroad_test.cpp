#include <iostream>

#include "emulator.h"

using std::cout; 
using std::endl;
using std::string; 

static int leftLed = 12; 
static int rightLed = 11; 

void test_pinchange(int pin, const PinState &prev, const PinState &next) {
  if (prev == next)
    return;

  if (pin != leftLed && pin != rightLed) 
    throw "Illegial pin used";

  if (Arduino.get_pin(leftLed).is_high() && Arduino.get_pin(rightLed).is_low()) {
    cout << "Left" << endl;
  }

  if (Arduino.get_pin(leftLed).is_low() && Arduino.get_pin(rightLed).is_high()) {
    cout << "Right" << endl;
  }
  
}

bool test_loop(int count) {
  cout << "loop #" << count << endl;
  return (count < 4);
}

void test_exit(void) {
  cout << "TEST: exit()" << endl;
}

