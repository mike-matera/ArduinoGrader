
#include "PinState.h"

std::ostream & operator<<(std::ostream &os, const PinState & me) {
  os << "mode: " << me.mode << " value: " << me.value; 
  return os;
}

bool operator==(const PinState &lhs, const PinState &rhs) {
  return (lhs.mode == rhs.mode && lhs.value == rhs.value);
}
