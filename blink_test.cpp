#include <stdio.h>

#include <iostream>

#include "Emulator.h"

static int led_pin = 13; 
bool turn_on = false;
bool turn_off = false;

void test_init(void) {
  printf("Hello Emulator World\n");
  Arduino.setPinWatcher([] (int pin, DigitalPin val) {
      if (pin == 13 && val.isHigh()) {
	std::cout << "LED is ON" << std::endl;
	turn_on = true;
      }

      if (pin == 13 && val.isLow()) {
	std::cout << "LED is OFF" << std::endl;
	turn_off = true;
      }
    });
}

bool test_loop_done() {
  static int count = 0;
  count++;
  return (count < 2 && !(turn_on && turn_off));
}

/*
void test_setup_done(void) {
}

*/

void test_exit(void) {
  if (turn_on && turn_off) {
    std::cout << "Test passed!" << std::endl;
  }else{
    std::cout << "Test failed!" << std::endl;
  }    
}
