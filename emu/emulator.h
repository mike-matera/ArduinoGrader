#ifndef EMULATOR_H 
#define EMULATOR_H 

#include <map>
#include <string>
#include <iostream>

using std::string; 
using std::endl; 
using std::cout; 

#define NUMPINS    20 
#define NUMANPINS  8

enum PinMode {
  kOutput, kInput, kPullup, kPWM, kSound, kAnalog
};

// Emulator Properties
// For baud rate and misc. 
string emu_get_property(const string &key); 
void emu_set_property(const string &key, const string &val);

// Pin access.
void emu_set_pinmode(int pin, PinMode mode); 
void emu_set_pinvalue(int pin, int value);   
PinMode emu_get_pinmode(int pin);
int emu_get_pinvalue(int pin); 

// Test control
void test_start(void);
void test_setup(void);
bool test_loop(int);
void test_exit(void);
void test_check(const std::string &s);

#endif
