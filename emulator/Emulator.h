#ifndef EMULATOR_H
#define EMULATOR_H

#include <functional>
#include <iostream>
#include <vector>

#include "Arduino.h"
#include "PinState.h"

#define NUMPINS 20 

class Emulator {
  
public:

  Emulator() {
    for (int i=0; i<NUMPINS; i++) {
      pins.push_back(PinState());
    }
  }

  ~Emulator() {
  }

  void pinMode(int pin, uint8_t mode) {    
    PinState::PinMode m; 
    if (mode == INPUT) {
      m = PinState::PinMode::input;
    }else if (mode == OUTPUT) {
      m = PinState::PinMode::output; 
    }else{
      m = PinState::PinMode::pullup;
    }
    PinState prev = pins[pin];
    pins[pin].mode = m;
    if (watcher) 
      watcher(pin, prev, pins[pin]);
  }

  void digitalWrite(uint8_t pin, uint8_t value) {
    PinState prev = pins[pin];
    pins[pin].value = value;
    if (watcher) 
      watcher(pin, prev, pins[pin]);
  }

  void setPinWatcher(pinwatcher_t w) {
    watcher = w;
  }

private:
  std::vector<PinState> pins; 
  pinwatcher_t watcher;
};

extern Emulator Arduino; 

void test_setup(void);
bool test_loop(void);
void test_exit(void);
void test_check(const std::string &);

#endif
