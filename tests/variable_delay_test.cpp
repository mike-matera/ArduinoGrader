#include <climits>
#include <future>
#include <iostream>
#include <sstream>

#include "emulator.h"

using std::cout; 
using std::endl;
using std::string; 
using std::stringstream; 
using std::async;
using std::future;

static int ledPin = 0;

static stringstream input; 

volatile bool saw_400 = false;
volatile bool saw_600 = false;
volatile bool saw_800 = false;

volatile bool use_lf = false;
volatile bool start_input = false; 

void test_async() {
  // We don't know what to do with line endings
  // until after setup is run...
  while (! start_input) 
    Arduino.busy_wait(10);

  input << 200;
  if (use_lf)
    input << endl;

  while (!saw_400) 
    Arduino.busy_wait(10);

  input << 300; 
  if (use_lf) 
    input << endl;

  while (!saw_600) 
    Arduino.busy_wait(10);

  input << 400;
  if (use_lf) 
    input << endl;

}

void test_propchange(const string &prop, const string &value) {
  if (prop == "stream.timeout" && value == TO_STRING(ULONG_MAX)) {
    cout << "NOTE: stream.timeout seems to be set to -1: enabled NL" << endl;
    use_lf = true;
  }
}

void test_pinchange(int pin, const PinState &prev, const PinState &next) {
  static int blinkPeriod = 0; 
  static int lastTime = -1;

  if (prev == next)
    return;

  if (!prev.is_enabled() && next.is_enabled()) {
    ledPin = pin; 
    cout << "Enabled pin: " << pin << endl; 
  }

  if (pin == ledPin && prev.get_value() == HIGH && next.get_value() == LOW) {
    int t = Arduino.get_time() / 1000;
    int p = t - lastTime;
    cout << "Blink period: " << (t - lastTime) << endl;
    if (p == 400) {
      saw_400 = true;
    }else if (p == 600) {
      saw_600 = true;
    }else if (p == 800) {
      saw_800 = true;
    }
    lastTime = t;
    blinkPeriod = p;
  }
}

void test_setup() {
  cout << "TEST: test_setup()" << endl;
  Arduino.set_istream(&input);
}

bool test_loop(int count) {
  start_input = true;
  return !(saw_400 && saw_600 && saw_800) ;
}

void test_exit(void) {
  if (saw_400 && saw_600 && saw_800) {
    cout << "TEST PASSED" << endl;
  }else{
    cout << "TEST FAILED" << endl;
  }
}

void test_check(const std::string &what) {
  if (Arduino.get_time() > 10000000) {
    saw_400 = saw_600 = saw_800 = true;
    throw std::string("Simulator forced to exit after 10 seconds.");
  }
}
