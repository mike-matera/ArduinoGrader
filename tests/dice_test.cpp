#include <iostream>

#include "emulator.h"

using std::cout; 
using std::endl;
using std::string; 


void test_pinchange(int pin, const PinState &prev, const PinState &next) {
  cout << "TEST: test_pinchange(): pin: " << pin << " " << next << endl;
}

int test_getvalue(int pin, const PinState &state) {
  //cout << "TEST: test_getvalue(): " << pin << endl;
  long ts = Emulator::instance()->get_time() % 1000000;
  if (ts > 100000 && ts < 200000) {
    return LOW;
  }else{
    return HIGH;
  }
}

void test_setup(void) {
  cout << "TEST: test_setup()" << endl;
}

bool test_loop(int count) {
  return (Emulator::instance()->get_time() < 10000000);
}

void test_exit(void) {
  cout << "TEST: exit()" << endl;
}

void test_check(const std::string &what) {
  // if (what.substr(0,6) != "micros") { 
  //   std::cout << "TEST: trace: " << what << std::endl;
  // }
  if (Emulator::instance()->get_time() > 15000000) {
    throw std::string("Simulator forced to exit after 15 seconds.");
  }
}
