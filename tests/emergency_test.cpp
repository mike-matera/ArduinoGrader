#include <iostream>

#include "Emulator.h"

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
  
  int time = Arduino.getTime();
  if (prev.isLow() && next.isHigh()) {
    cout << "low  time: " << ((time - last)/1000) << endl;
  }
  if (prev.isHigh() && next.isLow()) {
    cout << "high time: " << ((time - last)/1000) << endl;
  }

  last = time;
}

bool test_loop(int count) {
  cout << "loop #" << count << endl;
  return (count < 3);
}
