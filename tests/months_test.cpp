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

void test_setup() {
  cout << "TEST: test_setup()" << endl;
  Arduino.set_istream(&input);
  input << "\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12";
}

bool test_loop(int count) {
  return (count < 12 && Arduino.get_time() < 2000000);
}

void test_exit(void) {
  cout << "TEST: exit()" << endl;
}

void test_check(const std::string &what) {
  if (Arduino.get_time() > 5000000) {
    throw std::string("Simulator forced to exit after 5 seconds.");
  }
}
