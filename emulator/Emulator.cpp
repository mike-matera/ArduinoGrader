#include <assert.h>


#ifndef __CYGWIN__
#include <execinfo.h>
#endif

#include <time.h>
#include <stdio.h>

#include <cstdarg>
#include <functional>
#include <string>
#include <vector>

#include "Arduino.h"
#include "Emulator.h"

Emulator Arduino; 

// helpers 

extern "C" {
  static void __test_setup(void){}
  static bool __test_loop(int){return true;}
  static void __test_exit(void){}
  static void __test_check(const std::string &s){}
  static void __test_pinchange(int, const PinState &, const PinState &) {}
  static void __test_propchange(const string &, const string &) {}
  static int __test_getvalue(int, const PinState &) {return 0;}
}

void test_setup(void) __attribute__((weak, alias("__test_setup"))) ;
bool test_loop(int) __attribute__((weak, alias("__test_loop"))) ;
void test_exit(void) __attribute__((weak, alias("__test_exit"))) ;
void test_check(const std::string &) __attribute__((weak, alias("__test_check"))) ;
void test_pinchange(int, const PinState &, const PinState &) __attribute__((weak, alias("__test_pinchange"))) ;
void test_propchange(const string &, const string &) __attribute__((weak, alias("__test_propchange"))) ;
int test_getvalue(int, const PinState &) __attribute__((weak, alias("__test_getvalue"))) ;

static struct timespec zerotime = { .tv_sec = 0, .tv_nsec = 0 } ; 

static void __check(const char *fmt...) {
  va_list args; 
  va_start(args, fmt);
  char buffer[256]; 
  vsnprintf(buffer, 256, fmt, args);
  test_check(buffer);
  va_end(args);
}


/* 
 * micros() on Arduino is microseconds of operation. No good code
 * should rely on it starting at zero. Most sketches can't hande a roll-over
 * which happens after about 40 days. Since the PC doesn't start at zero
 * at least I can prevent early roll over in 32 bits.
 */
unsigned long micros() {
  if (zerotime.tv_sec == 0) {
    clock_gettime(CLOCK_MONOTONIC, &zerotime);
  }
  struct timespec time; 
  clock_gettime(CLOCK_MONOTONIC, &time);
  time.tv_sec = time.tv_sec - zerotime.tv_sec; 
  time.tv_nsec = time.tv_nsec - zerotime.tv_nsec;
  int t = (time.tv_sec * 1000000 + time.tv_nsec / 1000); 
  __check("micros() == %d", t);
  return t;
}

unsigned long millis() {
  return micros()/1000;
}

void delayMicroseconds(unsigned int time) {
  int start = micros(); 
  int now; 
  do {
    now = micros();
  } while (now < start + time); 
}

void delay(unsigned long time) {
  delayMicroseconds(time * 1000);
}

void pinMode(uint8_t pin, uint8_t mode) {
  __check("pinMode(%d, %x)", pin, mode);
  assert(pin < NUMPINS);
  assert(mode == INPUT || mode == OUTPUT || mode == INPUT_PULLUP);  
  PinState p = Arduino.getPin(pin);
  PinState o = p;
  if (mode == INPUT) {
    p.setMode(PinMode::input);
  }else if (mode == OUTPUT) {
    p.setMode(PinMode::output);
  }else{
    p.setMode(PinMode::pullup);
  }
  Arduino.setPin(pin, p);
  test_pinchange(pin, o, p);
}

void digitalWrite(uint8_t pin, uint8_t value) {
  __check("digitalWrite(%d, %x)", pin, value);
  assert(pin < NUMPINS);
  assert(value == HIGH || value == LOW);
  PinState p = Arduino.getPin(pin);
  PinState o = p;
  p.setValue(value);
  Arduino.setPin(pin, p);
  test_pinchange(pin, o, p);
}

int digitalRead(uint8_t pin) {
  assert(pin < NUMPINS);
  PinState p = Arduino.getPin(pin);
  if (p.isInput()) {
    p.setValue(test_getvalue(pin, p));
  }
  __check("digitalRead(%d) == %x", pin, p.getValue());
  return p.getValue();
}

int analogRead(uint8_t pin) {
  assert(pin < NUMANPINS);
  pin += A0;
  PinState p = Arduino.getPin(pin);
  if (p.isInput()) {
    p.setValue(test_getvalue(pin, p));
  }
  __check("analogRead(%d) == %d", pin, p.getValue());
  return p.getValue();
}

void analogReference(uint8_t mode) {
  __check("analogReference(%x)", mode);
  Arduino.setProperty("analog.reference", TO_STRING(mode));
  test_propchange("analog.reference", TO_STRING(mode));
}

void analogWrite(uint8_t pin, int val) {
  __check("analogWrite(%d, %d)", pin, val);
  assert(pin < NUMPINS);
  val &= 0xff;
  PinState p = Arduino.getPin(pin);
  PinState o = p;
  p.setMode(PinMode::pwm);
  p.setValue(val);
  Arduino.setPin(pin, p);
  test_pinchange(pin, o, p);
}

#ifndef __CYGWIN__
#define STACKTRACE() {void *_sf[100]; backtrace_symbols_fd(_sf, backtrace(_sf, 100), fileno(stdout));}
#else
#define STACKTRACE() {}
#endif

int main(int argc, char **argv) {

  int loopcount = 0; 

  try {
    test_setup(); 
    __check("setup()");
    setup();
    
    while (test_loop(loopcount++)) {
      __check("loop() #%d", loopcount);
      loop();
    }

    test_exit();

  } catch (std::string s) {
    std::cout << "Emulator died during " << s << std::endl;
    STACKTRACE();

  } catch (...) {
    std::cout << "Emulator died because of an unknown exception." << std::endl;
    STACKTRACE();
  }
}
