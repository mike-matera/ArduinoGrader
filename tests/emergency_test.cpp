#include <iostream>

#include "emulator.h"

using std::cout; 
using std::endl;
using std::string; 

static int ledPin = 13; 

void test_pinchange(int pin, const PinState &prev, const PinState &next) {
  static int last = 0; 

  if (prev == next)
    return;
  
  if (pin != ledPin) 
    throw "Unexpected pin change!";
  
  int time = Arduino.get_time();
  if (prev.is_low() && next.is_high()) {
    cout << "low  time: " << ((time - last)/1000) << endl;
  }
  if (prev.is_high() && next.is_low()) {
    cout << "high time: " << ((time - last)/1000) << endl;
  }

  last = time;
}

bool test_loop(int count) {
  cout << "loop #" << count << endl;
  return (count < 3);
}
