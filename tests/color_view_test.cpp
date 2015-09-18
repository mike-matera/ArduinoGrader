#include <iostream>

#include "emulator.h"

using std::cout; 
using std::endl;
using std::string; 


static int redPin = 9; 
static int greenPin = 10; 
static int bluePin = 11; 
static int sensorPin = 0; 

void test_pinchange(int pin, const PinState &prev, const PinState &next) {
}

void test_propchange(const string &prop, const string &value) {
  cout << prop << " = " << value << endl;
}

int test_getvalue(int pin, const PinState &state) {
  
  float reflect = ((Arduino.get_pin(redPin).get_value() / 255.0) * 0.75 
		  + (Arduino.get_pin(greenPin).get_value() / 255.0) * 0.50
		  + (Arduino.get_pin(bluePin).get_value() / 255.0) * 0.25)
    * 1023.0;

  if (reflect > 1023)
    reflect = 1023; 

  return reflect;
}

void test_setup(void) {
  cout << "Sarting ColorView test.\n";
}

bool test_loop(int count) {
  return (count < 1);
}

void test_exit(void) {
  /*
  if (turn_on && turn_off) {
    std::cout << "Test passed!" << std::endl;
  }else{
    std::cout << "Test failed!" << std::endl;
  } 
  */
}
