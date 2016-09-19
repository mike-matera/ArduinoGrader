#ifndef FREQ_H_
#define FREQ_H_

/* A frequency counter for an Arduino pin
 */

#include "emulator.h" 

class FrequencyCounter : public PinHandler {

public: 

  FrequencyCounter() {
    valid_ = false; 
    mark_ = 0; 
    ontime_ = 0; 
    offtime_ = 0; 
  }

  virtual void onModeChange(PinState &pin, PinMode prev, PinMode next) {
    if (next == PinMode::kOutput) {
      valid_ = true;
    }else{
      valid_ = false;
    }
  }

  virtual void onValueChange(PinState& pin, int prev, int next) {
    if (valid_ && next != prev) {
      if (mark_ == 0) {
	mark_ = emu.get_time();
      }else{
	unsigned long newtime = emu.get_time(); 
	if (prev != 0) {
	  ontime_ = newtime - mark_;
	}else{
	  offtime_ = newtime - mark_; 
	}
	mark_ = newtime; 
      }
    }
  }

  unsigned long getOnTime() {
    return ontime_;
  }

  unsigned long getOffTime() {
    return offtime_; 
  }
  
  unsigned long getPeriod() {
    if (ontime_ == 0 || offtime_ == 0) 
      return 0;
    return ontime_ + offtime_;
  }
  
private:
  bool valid_; 
  unsigned long mark_; 
  unsigned long ontime_; 
  unsigned long offtime_; 
};

#endif
