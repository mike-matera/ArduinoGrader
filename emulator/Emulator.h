#ifndef EMULATOR_H
#define EMULATOR_H

#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "Arduino.h"
#include "PinState.h"

#define NUMPINS 20 

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
    for (int i=0; i<NUMPINS; i++) {
      pins.push_back(PinState());
    }
  }

  ~Emulator() {
    cout << "Emulator exiting. Properties list:" << endl;
    for (map<string,string>::iterator it = props.begin(); it != props.end(); it++) {
      cout << "\t" << it->first << " " << it->second << endl;
    }
  }

  PinState getPin(int num) {
    PinState p = pins[num];
    if (p.isInput() && producer) 
      p.setValue(producer(num, p));
    return(p);
  }

  void setPin(int num, PinState &p) {
    PinState prev = pins[num];
    pins[num] = p;
    if (watcher) 
      watcher(num, prev, pins[num]);
  }

  const map<string, string> getProperties() {
    return props; 
  }

  void setProperty(const string &key, const string &val) {
    props[key] = val;
  }

  void setPinWatcher(pinwatcher_t w) {
    watcher = w;
  }

  void setValueProducer(producer_t p) {
    producer = p;
  }

private:

  map<string, string> props;
  vector<PinState> pins; 
  pinwatcher_t watcher;
  producer_t producer;
};

extern Emulator Arduino; 

void test_setup(void);
bool test_loop(int);
void test_exit(void);
void test_check(const std::string &);

#endif
