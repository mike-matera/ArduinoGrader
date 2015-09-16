#ifndef PINSTATE_H
#define PINSTATE_H

#include <functional>
#include <iostream>
#include <vector>

#include "Arduino.h"

enum PinMode {
  output, input, pullup, pwm, sound, analog
};

class PinState {

public:

  PinState() {
    mode = PinMode::input; 
    value = 0; 
  }

  void setMode(PinMode m) {
    mode = m;
  }

  void setValue(int v) {
    value = v;
  }

  PinMode getMode() const {
    return mode; 
  }

  int getValue() const {
    return value; 
  }

  bool isEnabled() const {
    return (mode == output);
  }

  bool isHigh() const {
    return (mode == output && value == HIGH);
  }

  bool isLow() const {
    return (mode == output && value == LOW);
  }

  bool isInput() const {
    return (mode == input || mode == analog);
  }

  bool isPWM() const {
    return (mode == pwm);
  }

  friend std::ostream & operator<<(std::ostream &os, const PinState & me);
  friend bool operator==(const PinState &lhs, const PinState &rhs);

private:

  PinMode mode; 
  int value; 

};

std::ostream & operator<<(std::ostream &os, const PinState & me);
bool operator==(const PinState &lhs, const PinState &rhs);

#endif
