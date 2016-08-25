#include <assert.h>

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

#include <execinfo.h>

extern "C" {
  #include "stdlib.h"
}

#include "Arduino.h"
#include "emulator.h"

Emulator emu; 

const std::map<string, string> Emulator::get_properties() {
  return properties_; 
}

void Emulator::set_property(const string &key, const string &val) {
  properties_[key] = val;
}

unsigned long Emulator::get_time() const {
  struct timespec time; 
  clock_gettime(CLOCK_MONOTONIC, &time);
  time.tv_sec = time.tv_sec - start_time_.tv_sec; 
  time.tv_nsec = time.tv_nsec - start_time_.tv_nsec;
  return (time.tv_sec * 1000000 + time.tv_nsec / 1000); 
}

istream *Emulator::get_istream() {
  if (input_ == NULL) 
    return &std::cin;
  else
    return input_;
}

ostream *Emulator::get_ostream() {
  if (output_ == NULL) 
    return &std::cout;
  else
    return output_;
}

void Emulator::set_istream(istream *s) {
  input_ = s; 
}

void Emulator::set_ostream(ostream *s) {
  output_ = s; 
}

void Emulator::busy_wait(unsigned long usec) {
  unsigned long start = get_time();
  unsigned long wait = usec * 1000;
  while (get_time() < (start + wait)) {
    std::this_thread::yield();
  }
}

// helpers 
extern "C" {
  static void __test_start(void){}
  static void __test_setup(void){}
  static bool __test_loop(int){return true;}
  static void __test_exit(void){}
  static void __test_check(const std::string &s){}
}

void test_start(void) __attribute__((weak, alias("__test_start"))) ;
void test_setup(void) __attribute__((weak, alias("__test_setup"))) ;
bool test_loop(int) __attribute__((weak, alias("__test_loop"))) ;
void test_exit(void) __attribute__((weak, alias("__test_exit"))) ;
void test_check(const std::string &) __attribute__((weak, alias("__test_check"))) ;

void __check(const char *fmt...) {
  va_list args; 
  va_start(args, fmt);
  char buffer[256]; 
  vsnprintf(buffer, 256, fmt, args);
  va_end(args);
  test_check(buffer);
}

#define STACKTRACE() {\
    void *_sf[100]; \
    backtrace_symbols_fd(_sf, backtrace(_sf, 100), fileno(stdout)); \
  }

int main(int argc, char **argv) {

  int loopcount = 0; 

  try {

    test_start(); 

    __check("setup()");
    setup();
    test_setup(); 

    do {
      __check("loop() #%d", loopcount);
      loop();
    } while (test_loop(loopcount++));

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
