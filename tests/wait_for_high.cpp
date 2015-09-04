#include <iostream>

#include "Emulator.h"
#include "Arduino.h"

void test_setup() {
  Arduino.setValueProducer([] (int num, PinState p) {
      if (millis() < 5000)
	return LOW;
      return HIGH;
    });
}

bool test_loop(int count) {
  return (count < 1);
}
