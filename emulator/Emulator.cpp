#include <assert.h>

#ifndef __CYGWIN__
#include <execinfo.h>
#endif

#include <time.h>
#include <stdio.h>

#include <atomic>
#include <cstdarg>
#include <future>
#include <functional>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "Arduino.h"
#include "Emulator.h"

using std::atomic_bool;
using std::async;
using std::future;

Emulator Arduino; 

static std::mutex state_mutex;

#define LOCK() state_mutex.lock()
#define UNLOCK() state_mutex.unlock()

PinState Emulator::getPin(int num) {
  return pins[num];
}

void Emulator::setPin(int num, PinState &p) {
  pins[num] = p;
}

const map<string, string> Emulator::getProperties() {
  return props; 
}

void Emulator::setProperty(const string &key, const string &val) {
  props[key] = val;
}

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
  unsigned long t = Arduino.getTime();
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


static void __set_and_call(int pin, PinMode mode, int value) {
  LOCK();
  PinState p = Arduino.getPin(pin);
  PinState o = p;
  p.setMode(mode);
  p.setValue(value);
  Arduino.setPin(pin, p);
  UNLOCK();
  test_pinchange(pin, o, p);
}

static void __set_and_call(int pin, PinMode mode) {
  LOCK();
  PinState p = Arduino.getPin(pin);
  PinState o = p;
  p.setMode(mode);
  Arduino.setPin(pin, p);
  UNLOCK();
  test_pinchange(pin, o, p);
}

static void __set_and_call(int pin, int value) {
  LOCK();
  PinState p = Arduino.getPin(pin);
  PinState o = p;
  p.setValue(value);
  Arduino.setPin(pin, p);
  UNLOCK();
  test_pinchange(pin, o, p);
}

void pinMode(uint8_t pin, uint8_t mode) {
  __check("pinMode(%d, %x)", pin, mode);
  assert(pin < NUMPINS);
  assert(mode == INPUT || mode == OUTPUT || mode == INPUT_PULLUP);  

  if (mode == INPUT) {
    __set_and_call(pin, PinMode::input);
  }else if (mode == OUTPUT) {
    __set_and_call(pin, PinMode::output);
  }else{
    __set_and_call(pin, PinMode::pullup);
  }
}

void digitalWrite(uint8_t pin, uint8_t value) {
  __check("digitalWrite(%d, %x)", pin, value);
  assert(pin < NUMPINS);
  assert(value == HIGH || value == LOW);
  __set_and_call(pin, value);
}

int digitalRead(uint8_t pin) {
  assert(pin < NUMPINS);
  LOCK();
  PinState p = Arduino.getPin(pin);
  bool input = p.isInput();
  int value = 0;
  UNLOCK();
  if (input) {
    value = test_getvalue(pin, p);
  }
  __check("digitalRead(%d) == %x", pin, value);
  return value;
}

int analogRead(uint8_t pin) {
  assert(pin < NUMANPINS);
  pin += A0;
  LOCK();
  PinState p = Arduino.getPin(pin);
  bool input = p.isInput();
  int value = 0;
  UNLOCK();
  if (input) {
    value = test_getvalue(pin, p);
  }
  __check("analogRead(%d) == %d", pin, value);
  return value;
}

void analogReference(uint8_t mode) {
  __check("analogReference(%x)", mode);
  LOCK();
  Arduino.setProperty("analog.reference", TO_STRING(mode));
  UNLOCK();
  test_propchange("analog.reference", TO_STRING(mode));
}

void analogWrite(uint8_t pin, int val) {
  __check("analogWrite(%d, %d)", pin, val);
  assert(pin < NUMPINS);
  val &= 0xff;
  __set_and_call(pin, PinMode::pwm, val);
}

static future<void> tone_future; 
static atomic_bool future_pending;

void tone(uint8_t pin, unsigned int frequency, unsigned long duration) {
  __check("tone(%d, %d, %d)", pin, frequency, duration);
  assert(pin < NUMPINS);

  // If there's a thread waiting it must be canceled before we set a 
  // new tone...
  if (future_pending) {
    future_pending = false;
    tone_future.wait();
  }

  if (duration != 0) {
    // Set a thread to wait for the duration and turn off the tone...
    unsigned long offtime = (Arduino.getTime()/1000) + duration;
    future_pending = true;
    tone_future = async(std::launch::async, [=] () {
	while (future_pending && (Arduino.getTime()/1000) < offtime) {
	  std::this_thread::yield();
	}
	if (! future_pending) {
	  return;
	}
	__set_and_call(pin, PinMode::sound, 0);
	future_pending = false;
      });
  }
  __set_and_call(pin, PinMode::sound, frequency);
}

void noTone(uint8_t pin) {
  __check("noTone(%d)", pin);
  if (future_pending) {
    future_pending = false;
    tone_future.wait();
  }
  __set_and_call(pin, PinMode::sound, 0);
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

  } catch (const char *c) {
    std::cout << "Emulator caught an exception:" << std::endl
	      << "\t" << c << endl;
    STACKTRACE();

  } catch (const std::string &s) {
    std::cout << "Emulator caught an exception:" << std::endl
	      << "\t" << s << endl;
    STACKTRACE();

  } catch (...) {
    std::cout << "Emulator died because of an unknown exception." << std::endl;
    STACKTRACE();
  }
}
