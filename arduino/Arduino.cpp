#include "Arduino.h"
#include "emulator.h"

#include <assert.h>

#include <atomic>
#include <cstdarg>
#include <future>
#include <functional>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

using std::atomic_bool;
using std::async;
using std::future;

static future<void> tone_future; 
static atomic_bool future_pending;

void pinMode(uint8_t pin, uint8_t mode) {
  assert(pin < NUMPINS);
  if (mode == INPUT) {
    emu_set_pinmode(pin, PinMode::kInput);
  }else if (mode == OUTPUT) {
    emu_set_pinmode(pin, PinMode::kOutput);
  }else if (mode == INPUT_PULLUP) {
    emu_set_pinmode(pin, PinMode::kPullup);
  }else{
    cout << "INVALID PIN MODE" << endl;
    assert(false);
  }
}

void digitalWrite(uint8_t pin, uint8_t value) {
  assert(pin < NUMPINS);
  assert(value == HIGH || value == LOW);
  emu_set_pinvalue(pin, value);
}

int digitalRead(uint8_t pin) {
  assert(pin < NUMPINS);
  int value = emu_get_pinvalue(pin);
  return value;
}

// FIXME: What should happen when you try to read a pin that's not in analog mode?
int analogRead(uint8_t pin) {
  assert(pin < NUMANPINS);
  pin += A0;
  int value = emu_get_pinvalue(pin);
  return value;
}

void analogReference(uint8_t mode) {
  emu_set_property("analog.reference", std::to_string(mode));
}

void analogWrite(uint8_t pin, int val) {
  assert(pin < NUMPINS);
  val &= 0xff;
  emu_set_pinmode(pin, PinMode::kPWM);
  emu_set_pinvalue(pin, val);
}

void tone(uint8_t pin, unsigned int frequency, unsigned long duration) {
  assert(pin < NUMPINS);

  emu_set_pinmode(pin, PinMode::kSound);

  // If there's a thread waiting it must be canceled before we set a 
  // new tone...
  if (future_pending) {
    future_pending = false;
    tone_future.wait();
  }

  if (duration != 0) {
    // Set a thread to wait for the duration and turn off the tone...
    unsigned long offtime = (get_time()/1000) + duration;
    future_pending = true;
    tone_future = async(std::launch::async, [=] () {
	while (future_pending && (get_time()/1000) < offtime) {
	  std::this_thread::yield();
	}
	if (! future_pending) {
	  return;
	}

	emu_set_pinvalue(pin, 0);

	future_pending = false;
      });
  }

  emu_set_pinvalue(pin, frequency);
}

void noTone(uint8_t pin) {
  if (future_pending) {
    future_pending = false;
    tone_future.wait();
  }
  emu_set_pinmode(pin, PinMode::kSound);
  emu_set_pinvalue(pin, 0);
}

void randomSeed(unsigned long seed)
{
  if (seed != 0) {
    srandom(seed);
  }
}

long random(long howbig)
{
  if (howbig == 0) {
    return 0;
  }
  return random() % howbig;
}

long random(long howsmall, long howbig)
{
  if (howsmall >= howbig) {
    return howsmall;
  }
  long diff = howbig - howsmall;
  return random(diff) + howsmall;
}

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/* 
 * micros() on Arduino is microseconds of operation. No good code
 * should rely on it starting at zero. Most sketches can't hande a roll-over
 * which happens after about 40 days. Since the PC doesn't start at zero
 * at least I can prevent early roll over in 32 bits.
 */
unsigned long micros() {
  unsigned long t = get_time();
  return t;
}

unsigned long millis() {
  return micros()/1000;
}

void delayMicroseconds(unsigned int time) {
  unsigned long start = micros(); 
  unsigned long now; 
  do {
    now = micros();
  } while (now < start + time); 
}

void delay(unsigned long time) {
  delayMicroseconds(time * 1000);
}

void busy_wait(unsigned long usec) {
  unsigned long start = get_time();
  unsigned long wait = usec * 1000;
  while (get_time() < (start + wait)) {
    std::this_thread::yield();
  }
}
