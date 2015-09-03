#ifndef PINSTATE_H
#define PINSTATE_H

#include <functional>
#include <iostream>
#include <vector>

#include "Arduino.h"

class PinState {
  friend class Emulator; 

public:
  enum PinMode {
    output, input, pullup, pwm, tone, analog
  };

  PinState() {
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

  friend std::ostream & operator<<(std::ostream &os, const PinState & me);
  friend bool operator==(const PinState &lhs, const PinState &rhs);

protected:

  PinMode mode; 
  int value; 

};

std::ostream & operator<<(std::ostream &os, const PinState & me);
bool operator==(const PinState &lhs, const PinState &rhs);

typedef std::function<void (int, PinState, PinState)> pinwatcher_t ; 

#endif
