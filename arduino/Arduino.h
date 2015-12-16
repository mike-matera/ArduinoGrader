/*
 * This is a hacked Arduino emulator
 *
 */
#ifndef ARDUINO_H
#define ARDUINO_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"{
#endif

#define F_CPU 8000000UL

#define HIGH 0x1
#define LOW  0x0

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2

#define DEFAULT 0
#define EXTERNAL 1
#define INTERNAL 2

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105
#define EULER 2.718281828459045235360287471352

#define SERIAL  0x0
#define DISPLAY 0x1

#define LSBFIRST 0
#define MSBFIRST 1

#define CHANGE 1
#define FALLING 2
#define RISING 3

#define LED_BUILTIN 13 

  static const uint8_t A0 = 14;
  static const uint8_t A1 = 15;
  static const uint8_t A2 = 16;
  static const uint8_t A3 = 17;
  static const uint8_t A4 = 18;
  static const uint8_t A5 = 19;
  static const uint8_t A6 = 20;
  static const uint8_t A7 = 21;

  void setup(void);
  void loop(void);

  unsigned long millis(void); 
  unsigned long micros(void);
  void delay(unsigned long);
  void delayMicroseconds(unsigned int us);

  void pinMode(uint8_t, uint8_t);
  void digitalWrite(uint8_t, uint8_t);
  int digitalRead(uint8_t);
  int analogRead(uint8_t);
  void analogReference(uint8_t mode);
  void analogWrite(uint8_t, int);

  void tone(uint8_t _pin, unsigned int frequency, unsigned long duration = 0);
  void noTone(uint8_t _pin);

#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)

#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

#define clockCyclesPerMicrosecond() ( F_CPU / 1000000L )
#define clockCyclesToMicroseconds(a) ( (a) / clockCyclesPerMicrosecond() )
#define microsecondsToClockCycles(a) ( (a) * clockCyclesPerMicrosecond() )

#ifdef __cplusplus
}
#endif

long random(long);
long random(long, long);
void randomSeed(unsigned long);
long map(long, long, long, long, long);

#include "Serial.h"

void __check(const char *fmt...);

// byte doesn't seem to be available on my host...
typedef uint8_t byte;

#endif
