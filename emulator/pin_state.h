#ifndef PINSTATE_H
#define PINSTATE_H

#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "Arduino.h"

using std::vector; 
using std::string; 
using std::function; 
using std::reference_wrapper; 

class PinState; 

typedef vector<reference_wrapper<PinState> > registration; 

enum PinMode {
  kOutput, kInput, kPullup, kPWM, kSound, kAnalog
};

class PinHandler {
  friend class Emulator; 

public:

  PinHandler() {
  }

  virtual ~PinHandler() {
  }

  virtual void onModeChange(PinState& pin, PinMode prev, PinMode next) {} 
  virtual void onValueChange(PinState& pin, int prev, int next) {}
  virtual int onValueNeeded(PinState& pin) {return 0;}

protected:

  registration reg_; 

};

class PinState {
  friend class Emulator; 

public:

  PinState() {
    mode_ = PinMode::kInput; 
    value_ = 0; 
    handler_ = NULL; 
    name_ = "";
  }

  void set_mode(PinMode m) {
    if (handler_ != NULL) 
      handler_->onModeChange(*this, mode_, m);
    mode_ = m;
  }

  void set_value(int v) {
    if (handler_ != NULL)
      handler_->onValueChange(*this, value_, v);
    value_ = v;
  }

  PinMode get_mode() const {
    return mode_; 
  }

  int get_value() {
    if (handler_ != NULL) 
      return handler_->onValueNeeded(*this);
    return value_; 
  }

  void set_name(const string &n) {
    name_ = n; 
  }

  const string &get_name() const {
    return name_; 
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
  PinHandler *handler_;
  string name_;
};

std::ostream & operator<<(std::ostream &os, const PinState & me);
bool operator==(const PinState &lhs, const PinState &rhs);

#endif
