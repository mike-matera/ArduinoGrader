#include <iostream>

#include "Emulator.h"
#include "Arduino.h"

int test_getvalue(int pin, const PinState &state) {
  if (millis() < 1000)
    return LOW;
  return HIGH;
}

bool test_loop(int count) {
  return (count < 1);
}
