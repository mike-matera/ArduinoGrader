#ifndef PINSTATE_H
#define PINSTATE_H

#include <functional>
#include <iostream>
#include <vector>

#include "Arduino.h"

enum PinMode {
  kOutput, kInput, kPullup, kPWM, kSound, kAnalog
};

class PinState {

public:

  PinState() {
    mode_ = PinMode::kInput; 
    value_ = 0; 
  }

  void set_mode(PinMode m) {
    mode_ = m;
  }

  void set_value(int v) {
    value_ = v;
  }

  PinMode get_mode() const {
    return mode_; 
  }

  int get_value() const {
    return value_; 
  }

  bool is_enabled() const {
    return (mode_ == kOutput);
  }

  bool is_high() const {
    return (mode_ == kOutput && value_ == HIGH);
  }

  bool is_low() const {
    return (mode_ == kOutput && value_ == LOW);
  }

  bool is_input() const {
    return (mode_ == kInput || mode_ == kPullup);
  }

  bool is_analog() const {
    return (mode_ == kInput);
  }

  bool is_PWM() const {
    return (mode_ == kPWM);
  }

  friend std::ostream & operator<<(std::ostream &os, const PinState & me);
  friend bool operator==(const PinState &lhs, const PinState &rhs);

private:

  PinMode mode_; 
  int value_; 

};

std::ostream & operator<<(std::ostream &os, const PinState & me);
bool operator==(const PinState &lhs, const PinState &rhs);

#endif
