#include <iostream>
#include <sstream>

#include "emulator.h"

using std::cout; 
using std::endl;
using std::string; 
using std::stringstream; 

static int ledPin = 0;

static stringstream input; 

void test_pinchange(int pin, const PinState &prev, const PinState &next) {
  static int blinkPeriod = 0; 
  static int lastTime = -1;

  if (prev == next)
    return;

  if (!prev.is_enabled() && next.is_enabled()) {
    ledPin = pin; 
    cout << "Enabled pin: " << pin; 
  }

  if (pin == ledPin && prev.get_value() == HIGH && next.get_value() == LOW) {
    int t = Arduino.get_time() / 1000;
    int p = t - lastTime;
    //if (p != blinkPeriod) {
      cout << "Blink period: " << (t - lastTime) << endl;
      //}
    lastTime = t;
    blinkPeriod = p;
  }

}

void test_propchange(const string &prop, const string &value) {
  cout << "TEST: test_propchange(): " << prop << " = " << value << endl;
}

int test_getvalue(int pin, const PinState &state) {
  cout << "TEST: test_getvalue(): " << pin << endl;
  return 0;
}

void test_setup(void) {
  cout << "TEST: test_setup()" << endl;
  Arduino.set_istream(&input);
}

/*
bool test_loop(int count) {
  cout << "TEST: loop() #" << count << endl;
  if (count == 10) {
    input << "300" << endl;
  }
  return true;
}
*/

void test_exit(void) {
  cout << "TEST: exit()" << endl;
}

void test_check(const std::string &what) {
  if (Arduino.get_time() > 10000000) {
    throw std::string("Simulator forced to exit after 10 seconds.");
  }
}
