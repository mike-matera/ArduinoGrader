#include <iostream>

#include "Emulator.h"

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

  if (Arduino.getPin(leftLed).isHigh() && Arduino.getPin(rightLed).isLow()) {
    cout << "Left" << endl;
  }

  if (Arduino.getPin(leftLed).isLow() && Arduino.getPin(rightLed).isHigh()) {
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

