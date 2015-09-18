#include <stdio.h>

#include <iostream>

#include "emulator.h"

using std::cout; 
using std::endl;
using std::string; 

static int led_pin = 13; 
bool turn_on = false;
bool turn_off = false;

void test_pinchange(int pin, const PinState &prev, const PinState &next) {
  if (prev == next) 
    return; 

  if (pin == 13 && next.is_high()) {
    std::cout << "LED is ON" << std::endl;
    turn_on = true;
  }

  if (pin == 13 && next.is_low()) {
    std::cout << "LED is OFF" << std::endl;
    turn_off = true;
  }
}

void test_propchange(const string &prop, const string &value) {
  cout << prop << " = " << value << endl;
}

int test_getvalue(int pin, const PinState &state) {
  cout << "Unexpected read of pin: " << pin << endl;
  return 0;
}

void test_setup(void) {
  printf("Hello Emulator World\n");
}

bool test_loop(int count) {
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
