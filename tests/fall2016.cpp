
#include <iostream>

#include "emulator.h"

using std::cout; 
using std::endl;
using std::string; 

#define LED0 2 
#define LED1 4 
#define LED2 7 
#define LED3 8 

class StatusLEDs : public PinHandler {
public:
  virtual void value_change(PinState &pin, int o, int n) {
    cout << "LED value change!" << endl;
  }
  virtual void mode_change(PinState &pin, PinMode o, PinMode n) {
    cout << "LED mode change!" << endl;
  };
  virtual int get_value(PinState &pin) {
    cout << "Read LED value???" << endl;
    return 0;
  }

  registration pins_;   
};

StatusLEDs leds; 

void test_start(void) {
  emu.reg_callback({LED0, LED1, LED2, LED3}, &leds);
  cout << "TEST: test_start()" << endl;
}

void test_setup(void) {
  cout << "TEST: test_setup()" << endl;
  
}

bool test_loop(int count) {
  cout << "TEST: loop()" << endl;
  return (count < 1);
}

void test_exit(void) {
  cout << "TEST: exit()" << endl;
}

/*
void test_check(const std::string &what) {
  if (what.substr(0,6) != "micros") { 
    std::cout << "TEST: trace: " << what << std::endl;
  }
  if (emu.get_time() > 5000000) {
    throw std::string("Simulator forced to exit after 5 seconds.");
  }
}
*/
