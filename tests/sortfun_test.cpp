#include <climits>
#include <iostream>
#include <sstream>

#include "emulator.h"

using std::cout; 
using std::endl;
using std::flush;
using std::string; 
using std::stringstream; 

static stringstream input; 

void test_propchange(const string &prop, const string &value) {
  if (prop == "stream.timeout" && value == TO_STRING(ULONG_MAX)) {
    cout << "NOTE: stream.timeout seems to be set to -1: enabled NL" << endl;
    input << "\n";
  }
}

int test_getvalue(int pin, const PinState &state) {
  cout << "TEST: test_getvalue(): " << pin << endl;
  return 0;
}

void test_setup() {
  cout << "TEST: test_setup()" << endl;
  Emulator::instance()->set_istream(&input);
  input << "10.2" << endl;
  input << "12.6" << endl; 
  input << "1.2" << endl;
  input << "0.6" << endl; 
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
    //std::cout << "TEST: trace: " << what << std::endl;
  }
  if (Emulator::instance()->get_time() > 60000000) {
    throw std::string("Simulator forced to exit after 60 seconds.");
  }
}
