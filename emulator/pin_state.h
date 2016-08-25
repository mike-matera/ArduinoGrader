#ifndef PINSTATE_H
#define PINSTATE_H

#include <functional>
#include <iostream>
#include <vector>

#include "Arduino.h"

using std::vector; 
using std::function; 

class PinState; 

enum PinMode {
  kOutput, kInput, kPullup, kPWM, kSound, kAnalog
};

typedef function<void(PinState&, PinMode, PinMode)> mode_handler;
typedef function<void(PinState&, int, int)> value_handler;
typedef function<int(PinState&)> value_producer;

class PinState {
  friend class Emulator; 

public:

  PinState() {
    mode_ = PinMode::kInput; 
    value_ = 0; 
  }

  void set_mode(PinMode m) {
    if (mh_) 
      mh_(*this, mode_, m);
    mode_ = m;
  }

  void set_value(int v) {
    if (vh_) 
      vh_(*this, value_, v);
    value_ = v;
  }

  PinMode get_mode() const {
    return mode_; 
  }

  int get_value() {
    if (vp_) 
      return vp_(*this);
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

  mode_handler mh_; 
  value_handler vh_; 
  value_producer vp_;
};

std::ostream & operator<<(std::ostream &os, const PinState & me);
bool operator==(const PinState &lhs, const PinState &rhs);

#endif
