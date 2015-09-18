#include <iostream>

#include "Arduino.h"
#include "emulator.h"

int test_getvalue(int pin, const PinState &state) {
  if (millis() < 1000)
    return LOW;
  return HIGH;
}

bool test_loop(int count) {
  return (count < 1);
}
