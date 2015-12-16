#include <iostream>
#include <sstream>

#include "emulator.h"

using std::cout; 
using std::endl;
using std::string; 
using std::stringstream;

void test_pinchange(int pin, const PinState &prev, const PinState &next) {
  if (prev == next)
    return;
  cout << "TEST: test_pinchange(): pin: " << pin << " " << next << endl;
}

void test_propchange(const string &prop, const string &value) {
  cout << "TEST: test_propchange(): " << prop << " = " << value << endl;
}

void test_setup(void) {
  static stringstream input; 
  cout << "TEST: test_setup()" << endl;
  Arduino.set_istream(&input);
  input << "-1 3 7 -11 -2 5 -6 10 0\n";
}

bool test_loop(int count) {
  cout << "TEST: loop()" << endl;
  return (count < 1);
}

void test_exit(void) {
  cout << "TEST: exit()" << endl;
}

void test_check(const std::string &what) {
  if (what.substr(0,6) != "micros") { 
    if (what.substr(0,7) == "Servo::") {
      std::cout << "TEST: trace: " << what << std::endl;
    }
  }
  if (Arduino.get_time() > 50000000) {
    throw std::string("Simulator forced to exit after 50 seconds.");
  }
}
