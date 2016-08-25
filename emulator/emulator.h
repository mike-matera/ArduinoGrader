#ifndef EMULATOR_H
#define EMULATOR_H

#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <thread>
#include <vector>

#include "Arduino.h"
#include "pin_state.h"

#define NUMPINS    20 
#define NUMANPINS  8

using std::vector; 
using std::string; 
using std::endl; 
using std::cout; 
using std::istream; 
using std::ostream; 
using std::reference_wrapper; 

typedef vector<reference_wrapper<PinState> > registration; 

class Emulator {

public:

  enum Change {
    kType  = 1, 
    kValue = 2, 
    kRead  = 4,
    kProp  = 8,
  };

  Emulator() {
    input_ = NULL;
    output_ = NULL;
    clock_gettime(CLOCK_MONOTONIC, &start_time_);
    for (int i=0; i<NUMPINS; i++) {
      pins_.push_back(PinState());
    }
  }

  ~Emulator() {
    cout << "Emulator exiting. Properties list:" << endl;
    for (std::map<string,string>::iterator it = properties_.begin(); it != properties_.end(); it++) {
      cout << "\t" << it->first << " " << it->second << endl;
    }
  }

  PinState& operator[] (std::size_t i) {
    return pins_[i];
  }

  const std::map<string, string> get_properties();
  void set_property(const string &key, const string &val);
  unsigned long get_time() const;
  istream *get_istream();
  ostream *get_ostream();
  void set_istream(istream *s);
  void set_ostream(ostream *s);
  void busy_wait(unsigned long usec);

  // Test interface 
  registration reg_callback(vector<int> pins, mode_handler mh, value_handler vh, value_producer vp) {
    registration r; 
    for (auto &p : pins) {
      pins_[p].mh_ = mh;
      r.push_back(pins_[p]);
    }
    return r;
  }

private:

  struct timespec start_time_;
  std::map<string, string> properties_;
  vector<PinState> pins_; 
  istream *input_;
  ostream *output_; 
};

extern Emulator emu; 

void __check(const char *fmt...);

#endif
