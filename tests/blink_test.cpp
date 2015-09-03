#include <stdio.h>

#include <iostream>

#include "Emulator.h"

static int led_pin = 13; 
bool turn_on = false;
bool turn_off = false;

void test_setup(void) {
  printf("Hello Emulator World\n");
  Arduino.setPinWatcher([] (int pin, PinState prev, PinState next) {
      if (prev == next) 
	return; 

      if (pin == 13 && next.isHigh()) {
	std::cout << "LED is ON" << std::endl;
	turn_on = true;
      }

      if (pin == 13 && next.isLow()) {
	std::cout << "LED is OFF" << std::endl;
	turn_off = true;
      }
    });
}

bool test_loop() {
  static int count = 0;
  count++;
  return (count < 4 && !(turn_on && turn_off));
}

void test_exit(void) {
  if (turn_on && turn_off) {
    std::cout << "Test passed!" << std::endl;
  }else{
    std::cout << "Test failed!" << std::endl;
  }    
}

void test_check(const std::string &what) {
  if (what.substr(0,6) != "micros") { 
    std::cout << what << std::endl;
  }
}
