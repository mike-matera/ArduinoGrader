#include <iostream>

#include "emulator.h"

using std::cout; 
using std::endl;
using std::string; 

void test_async() {
}

void test_pinchange(int pin, const PinState &prev, const PinState &next) {
  if (prev == next)
    return;
  cout << "TEST: test_pinchange(): pin: " << pin << " " << next << endl;
}

void test_propchange(const string &prop, const string &value) {
  cout << "TEST: test_propchange(): " << prop << " = " << value << endl;
}

int test_getvalue(int pin, const PinState &state) {
}

void test_setup(void) {
  cout << "TEST: test_setup()" << endl;
}

bool test_loop(int count) {
  cout << "TEST: loop()" << endl;
  return (count < 2);
}

void test_exit(void) {
  cout << "TEST: exit()" << endl;
}

void test_check(const std::string &what) {
  if (what.substr(0,6) != "micros") { 
    std::cout << "TEST: trace: " << what << std::endl;
  }
  if (Arduino.get_time() > 60000000) {
    throw std::string("Simulator forced to exit after 60 seconds.");
  }
}
