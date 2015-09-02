#include <assert.h>
#include <time.h>

#include <functional>
#include <vector>

#include "Arduino.h"
#include "Emulator.h"

Emulator Arduino; 

// helpers 
static struct timespec zerotime = { .tv_sec = 0, .tv_nsec = 0 } ; 

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
  return (time.tv_sec * 1000000 + time.tv_nsec / 1000);
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
  assert(pin < NUMPINS);
  assert(mode == INPUT || mode == OUTPUT || mode == INPUT_PULLUP);
  Arduino.pinMode(pin, mode);
}

void digitalWrite(uint8_t pin, uint8_t value) {
  assert(pin < NUMPINS);
  assert(value == HIGH || value == LOW);
  Arduino.digitalWrite(pin, value);
}

int digitalRead(uint8_t) {
}

int analogRead(uint8_t) {
}

void analogReference(uint8_t mode) {
  // FIXME: record this value...
}

void analogWrite(uint8_t, int) {
}

extern "C" {
static void __test_init(void){}
static void __test_setup_done(void){}
static bool __test_loop_done(void){printf("fuck\n"); return true;}
static void __test_exit(void){}
}

void test_init(void) __attribute__((weak,  alias("__test_init"))) ;
void test_setup_done(void) __attribute__((weak, alias("__test_setup_done"))) ;
bool test_loop_done(void) __attribute__((weak, alias("__test_loop_done"))) ;
void test_exit(void) __attribute__((weak, alias("__test_exit"))) ;

int main(int argc, char **argv) {

  test_init(); 

  setup();
  test_setup_done();
  
  do {
    loop();
  } while (test_loop_done());

  test_exit();
}

std::ostream & operator<<(std::ostream &os, const DigitalPin & me) {
  os << "mode: " << me.mode << " value: " << me.value; 
  return os;
}
