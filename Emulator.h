#ifndef EMULATOR_H
#define EMULATOR_H

#include <functional>
#include <iostream>
#include <vector>

#include "Arduino.h"

#define NUMPINS 20 

class DigitalPin {
public:
  enum PinMode {
    output, input, pullup, pwm, tone
  };

  DigitalPin() {
    mode = PinMode::input; 
    value = 0; 
  }

  bool isEnabled() {
    return (mode == output);
  }

  bool isHigh() {
    return (mode == output && value == HIGH);
  }

  bool isLow() {
    return (mode == output && value == LOW);
  }

  PinMode mode; 
  int value; 
};

std::ostream & operator<<(std::ostream &os, const DigitalPin & me);

typedef std::function<void (int, DigitalPin)> pinwatcher_t ; 

class Emulator {
  
public:

  Emulator() {
    for (int i=0; i<NUMPINS; i++) {
      pins.push_back(DigitalPin());
    }
  }

  ~Emulator() {
  }

  void pinMode(int pin, uint8_t mode) {    
    DigitalPin::PinMode m; 
    if (mode == INPUT) {
      m = DigitalPin::PinMode::input;
    }else if (mode == OUTPUT) {
      m = DigitalPin::PinMode::output; 
    }else{
      m = DigitalPin::PinMode::pullup;
    }
    pins[pin].mode = m;
    if (watcher) 
      watcher(pin, pins[pin]);
  }

  void digitalWrite(uint8_t pin, uint8_t value) {
    pins[pin].value = value;
    if (watcher) 
      watcher(pin, pins[pin]);
  }

  void setPinWatcher(pinwatcher_t w) {
    watcher = w;
  }

private:
  std::vector<DigitalPin> pins; 
  pinwatcher_t watcher;
};

extern Emulator Arduino; 

void test_init(void);
void test_setup_done(void);
bool test_loop_done(void);
void test_exit(void);

#endif
