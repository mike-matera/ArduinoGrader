
#include "pin_state.h"

std::ostream & operator<<(std::ostream &os, const PinState & me) {
  String modename; 
  if (me.mode_ == kOutput) {
    os << "output: ";
    if (me.value_)
      os << "HIGH";
    else
      os << "LOW";
  }else if (me.mode_ == kInput) {
    os << "input";
  }else if (me.mode_ == kPullup) {
    os << "input/pullup";
  }else if (me.mode_ == kPWM) {
    os << "PWM: duty == " << me.value_;
  }else if (me.mode_ == kSound) {
    os << "tone: frequency == " << me.value_;
  }else if (me.mode_ == kAnalog) {
    os << "analog input";
  }
  return os;
}

bool operator==(const PinState &lhs, const PinState &rhs) {
  return (lhs.mode_ == rhs.mode_ && lhs.value_ == rhs.value_);
}
