#include <iostream>
#include <sstream>

#include "emulator.h"

using std::cout; 
using std::endl;
using std::string; 
using std::stringstream; 

void test_async() {
  cout << "TEST: test_async()" << endl;
}

void test_pinchange(int pin, const PinState &prev, const PinState &next) {
  if (prev == next)
    return;
  //cout << "TEST: test_pinchange(): pin: " << pin << " " << next << endl;
}

void test_propchange(const string &prop, const string &value) {
  cout << "TEST: test_propchange(): " << prop << " = " << value << endl;
}

int test_getvalue(int pin, const PinState &state) {
  int reading = rand() % 256; 
  cout << "TEST: test_getvalue(): " << pin << " == " << reading << endl;
  return reading;
}

void test_setup(void) {
  srand(time(NULL));
  static stringstream input; 
  cout << "TEST: test_setup()" << endl;
  Emulator::instance()->set_istream(&input);
  input << "c\nc\nc\n";
}

bool test_loop(int count) {
  cout << "TEST: loop()" << endl;
  return (count < 3);
}

void test_exit(void) {
  cout << "TEST: exit()" << endl;
}

void test_check(const std::string &what) {
  if (what.substr(0,6) != "micros") { 
    //std::cout << "TEST: trace: " << what << std::endl;
  }
  if (Emulator::instance()->get_time() > 60000000) {
    throw std::string("Simulator forced to exit after 60 seconds.");
  }
}
