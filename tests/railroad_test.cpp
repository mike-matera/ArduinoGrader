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

  
}

bool test_loop(int count) {
  return (count < 2);
}

void test_exit(void) {
  cout << "TEST: exit()" << endl;
}

