#include <iostream>

#include "emulator.h"

using std::cout; 
using std::endl;
using std::string; 

class LEDs : public PinHandler { 
public: 

  LEDs() {
  }

  int getLEDNumber() {
    int rval = 0; 
    for (PinState p : reg_) {
      if (p.is_enabled() && p.is_high()) {
	rval |= 1;
      }
      rval = rval << 1; 
    }
    return rval;
  }

} BoardLEDs;

void test_start(void) {
  cout << "TEST: test_start()" << endl;
  registration r = emu.reg_callback({2, 4, 7, 8}, BoardLEDs);
  r[0].get().set_name("LED[0]");
  r[1].get().set_name("LED[1]");
  r[2].get().set_name("LED[2]");
  r[3].get().set_name("LED[3]");
}

void test_setup(void) {
  cout << "TEST: test_setup()" << endl;
}

bool test_loop(int count) {
  cout << "TEST: loop()" << endl;
  cout << "LEDs are reading: " << BoardLEDs.getLEDNumber() << endl;
  return (count < 1);
}

void test_exit(void) {
  cout << "TEST: exit()" << endl;
}

void test_check(const std::string &what) {
  if (what.substr(0,6) != "micros") { 
    std::cout << "TEST: trace: " << what << std::endl;
  }
  if (emu.get_time() > 5000000) {
    throw std::string("Simulator forced to exit after 5 seconds.");
  }
}
