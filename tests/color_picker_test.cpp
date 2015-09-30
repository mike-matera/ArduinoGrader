#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>

#include "emulator.h"

using std::cout; 
using std::endl;
using std::string; 
using std::vector;
using std::stringstream; 

vector<int> pins; 
stringstream input; 
int sum = 0; 

void test_pinchange(int pin, const PinState &prev, const PinState &next) {
  if (prev == next)
    return;

  if (!prev.is_enabled() && next.is_enabled()) {
    cout << "Enabled pin: " << pin << endl;
    pins.push_back(pin);
    return;
  }
  
  if (std::find(pins.begin(), pins.end(), pin) != pins.end()) {
    cout << "pin: " << pin << " " << next << endl;
    sum += next.get_value();
  }  
}

void test_setup() {
  Arduino.set_istream(&input);
  input << "1 2 3" << endl; 
  input << "4 5 6" << endl; 
  input << "7 8 9" << endl; 
}

bool test_loop(int count) {
  return (sum != 45 && sum != 2250);
}

void test_exit() {
  if (sum == 45) {
    cout << "TEST PASSED (common cathode)" << endl;
  }else if (sum == 2250) {
    cout << "TEST PASSED (common anode)" << endl;
  }else{
    cout << "TEST FAILED" << endl;
  }
}

void test_check(const std::string &what) {
  if (Arduino.get_time() > 5000000) {
    throw std::string("Simulator forced to exit after 5 seconds.");
  }
}
