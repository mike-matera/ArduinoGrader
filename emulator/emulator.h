#ifndef EMULATOR_H
#define EMULATOR_H

#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "Arduino.h"
#include "pin_state.h"

#define NUMPINS    20 
#define NUMANPINS  8

using std::map; 
using std::vector; 
using std::string; 
using std::endl; 
using std::cout; 

#ifdef __CYGWIN__
#include <sstream>
namespace fix_cygwin {
template <typename T> std::string to_string( const T& n ) {
  std::ostringstream os;
  os << n;
  return os.str();
}
}
#define TO_STRING(v) fix_cygwin::to_string(v)
#else
#define TO_STRING(v) std::to_string(v)
#endif

class Emulator {
  
public:
  
  Emulator() {
    clock_gettime(CLOCK_MONOTONIC, &start_time_);
    for (int i=0; i<NUMPINS; i++) {
      pins_.push_back(PinState());
    }
  }

  ~Emulator() {
    cout << "Emulator exiting. Properties list:" << endl;
    for (map<string,string>::iterator it = properties_.begin(); it != properties_.end(); it++) {
      cout << "\t" << it->first << " " << it->second << endl;
    }
  }

  PinState get_pin(int num);
  void set_pin(int num, PinState &p);
  const map<string, string> get_properties();
  void set_property(const string &key, const string &val);

  unsigned long get_time() const {
    struct timespec time; 
    clock_gettime(CLOCK_MONOTONIC, &time);
    time.tv_sec = time.tv_sec - start_time_.tv_sec; 
    time.tv_nsec = time.tv_nsec - start_time_.tv_nsec;
    return (time.tv_sec * 1000000 + time.tv_nsec / 1000); 
  }

private:

  struct timespec start_time_;
  map<string, string> properties_;
  vector<PinState> pins_; 

};

extern Emulator Arduino; 

void test_setup(void);
bool test_loop(int);
void test_exit(void);
void test_check(const std::string &);
void test_pinchange(int, const PinState &, const PinState &);
void test_propchange(const string &, const string &);
int test_getvalue(int, const PinState &);

#endif