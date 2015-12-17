#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>

#include "emulator.h"

using std::cout; 
using std::endl;
using std::string; 
using std::find; 

static vector<int> leds; 
static vector<int> buttons; 
static vector<int> min({INT_MAX, INT_MAX, INT_MAX}); 
static vector<int> max({0, 0, 0}); 

static int press = 0; 

void test_pinchange(int pin, const PinState &prev, const PinState &next) {
  if (prev == next)
    return;

  if (!prev.is_enabled() && next.is_enabled()) {
    cout << "Enabled pin: " << pin << endl;
    leds.push_back(pin);
    return;
  }

  if (next.get_mode() == PinMode::kPullup) {
    cout << "Pullup on pin: " << pin << endl; 
    buttons.push_back(pin);
    return;
  }

  if (find(leds.begin(), leds.end(), pin) != leds.end()) {
    int val = next.get_value();
    if (val < min[press]) 
      min[press] = val;
    if (val > max[press]) 
      max[press] = val;
  }
  
}

int test_getvalue(int pin, const PinState &state) {  
  if (find(buttons.begin(), buttons.end(), pin) == buttons.end()) {
    throw "Reading button without a pullup!";
  }
  if (pin == buttons[press]) 
    return LOW;
  return HIGH;
}

bool test_loop(int count) {
  if ((count % 300) == 299) {
    cout << "LED: " << press << " min: " << min[press] << " max: " << max[press] << endl;
    press++;
  }

  return (press < 3); 
}

void test_exit(void) {
  if (min[0] == 0 && min[1] == 0 && min[2] == 0
      && max[0] == 255 && max[1] == 255 && max[2] == 255) {
    cout << "TEST PASSED" << endl;
  }else{
    cout << "TEST FAILED" << endl; 
  }
}

void test_check(const std::string &what) {
  if (Emulator::instance()->get_time() > 20000000) {
    throw std::string("Simulator forced to exit after 20 seconds.");
  }
}
