#ifndef PINSTATE_H
#define PINSTATE_H

#include <functional>
#include <iostream>
#include <vector>

#include "Arduino.h"

enum PinMode {
  output, input, pullup, pwm, tone, analog
};

class PinState {

public:

  PinState() {
    mode = PinMode::input; 
    value = 0; 
  }

  PinMode getMode() {
    return mode; 
  }

  void setMode(PinMode m) {
    mode = m;
  }

  int getValue() {
    return value; 
  }

  void setValue(int v) {
    value = v;
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

  bool isInput() {
    return (mode == input || mode == analog);
  }

  friend std::ostream & operator<<(std::ostream &os, const PinState & me);
  friend bool operator==(const PinState &lhs, const PinState &rhs);

private:

  PinMode mode; 
  int value; 

};

std::ostream & operator<<(std::ostream &os, const PinState & me);
bool operator==(const PinState &lhs, const PinState &rhs);

typedef std::function<void (int, PinState, PinState)> pinwatcher_t ; 
typedef std::function<int (int, PinState)> producer_t ; 

#endif
