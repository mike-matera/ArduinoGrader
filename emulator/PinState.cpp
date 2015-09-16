
#include "PinState.h"

std::ostream & operator<<(std::ostream &os, const PinState & me) {
  String modename; 
  if (me.mode == output) {
    os << "output: ";
    if (me.value)
      os << "HIGH";
    else
      os << "LOW";
  }else if (me.mode == input) {
    os << "input";
  }else if (me.mode == pullup) {
    os << "input/pullup";
  }else if (me.mode == pwm) {
    os << "PWM: duty == " << me.value;
  }else if (me.mode == sound) {
    os << "tone: frequency == " << me.value;
  }else if (me.mode == analog) {
    os << "analog input";
  }
  return os;
}

bool operator==(const PinState &lhs, const PinState &rhs) {
  return (lhs.mode == rhs.mode && lhs.value == rhs.value);
}
