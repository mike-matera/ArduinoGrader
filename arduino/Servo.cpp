/* Emulator implementation of Servo 
 */

#include "emulator.h"
#include "Servo.h"
#include "Arduino.h"

#define usToTicks(_us)    (( clockCyclesPerMicrosecond()* _us) / 8)     // converts microseconds to tick (assumes prescale of 8)  // 12 Aug 2009
#define ticksToUs(_ticks) (( (unsigned)_ticks * 8)/ clockCyclesPerMicrosecond() ) // converts from ticks back to microseconds

#define TRIM_DURATION       2                               // compensation ticks to trim adjust for d
static servo_t servos[MAX_SERVOS];                          // static array of servo structures
static volatile int8_t Channel[_Nbr_16timers ];             // counter for the servo being pulsed for each timer (or -1 if refresh interval)

uint8_t ServoCount = 0;                                     // the total number of attached servos

// convenience macros
#define SERVO_INDEX_TO_TIMER(_servo_nbr) ((timer16_Sequence_t)(_servo_nbr / SERVOS_PER_TIMER)) // returns the timer controlling this servo
#define SERVO_INDEX_TO_CHANNEL(_servo_nbr) (_servo_nbr % SERVOS_PER_TIMER)       // returns the index of the servo on this timer
#define SERVO_INDEX(_timer,_channel)  ((_timer*SERVOS_PER_TIMER) + _channel)     // macro to access servo index by timer and channel
#define SERVO(_timer,_channel)  (servos[SERVO_INDEX(_timer,_channel)])            // macro to access servo class by timer and channel

#define SERVO_MIN() (MIN_PULSE_WIDTH - this->min * 4)  // minimum value in uS for this servo
#define SERVO_MAX() (MAX_PULSE_WIDTH - this->max * 4)  // maximum value in uS for this servo

Servo::Servo() 
{
  servoIndex = 0; 
  if( ServoCount < MAX_SERVOS) {
    this->servoIndex = ServoCount++;                    // assign a servo index to this instance
    servos[this->servoIndex].ticks = usToTicks(DEFAULT_PULSE_WIDTH);   // store default values  - 12 Aug 2009
  }
  else
    this->servoIndex = INVALID_SERVO ;  // too many servos

  //__check("Servo::Servo(): number: %d", this->servoIndex);
}

// attach the given pin to the next free channel, sets pinMode, returns channel number or 0 if failure
uint8_t Servo::attach(int pin)
{
  return this->attach(pin, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
}

// as above but also sets min and max values for writes. 
uint8_t Servo::attach(int pin, int min, int max)
{
  if(this->servoIndex < MAX_SERVOS ) {
    pinMode( pin, OUTPUT) ;                                   // set servo pin to output
    servos[this->servoIndex].Pin.nbr = pin;
    // todo min/max check: abs(min - MIN_PULSE_WIDTH) /4 < 128
    this->min  = (MIN_PULSE_WIDTH - min)/4; //resolution of min/max is 4 uS
    this->max  = (MAX_PULSE_WIDTH - max)/4;

    /*
    // initialize the timer if it has not already been initialized
    timer16_Sequence_t timer = SERVO_INDEX_TO_TIMER(servoIndex);
    if(isTimerActive(timer) == false)
      initISR(timer);
    */
    servos[this->servoIndex].Pin.isActive = true;  // this must be set after the check for isTimerActive
  }
  __check("Servo::attach(%d, %d, %d) == %d", pin, min, max, this->servoIndex);
  return this->servoIndex ;
}

// if value is < 200 its treated as an angle, otherwise as pulse width in microseconds 
void Servo::detach() 
{
  servos[this->servoIndex].Pin.isActive = false;
  /*
  timer16_Sequence_t timer = SERVO_INDEX_TO_TIMER(servoIndex);
  if(isTimerActive(timer) == false) {
    finISR(timer);
  }
  */
  __check("Servo::detach() [this->servoIndex == %d]", this->servoIndex);
}

void Servo::write(int value)
{
  __check("Servo::write(%d)", value);
  if(value < MIN_PULSE_WIDTH)
  {  // treat values less than 544 as angles in degrees (valid values in microseconds are handled as microseconds)
    if(value < 0) value = 0;
    if(value > 180) value = 180;
    value = map(value, 0, 180, SERVO_MIN(),  SERVO_MAX());
  }
  this->writeMicroseconds(value);
}

// Write pulse width in microseconds 
void Servo::writeMicroseconds(int value)
{
  // calculate and store the values for the given channel
  byte channel = this->servoIndex;
  if( (channel < MAX_SERVOS) )   // ensure channel is valid
  {
    if( value < SERVO_MIN() )          // ensure pulse width is valid
      value = SERVO_MIN();
    else if( value > SERVO_MAX() )
      value = SERVO_MAX();

    value = value - TRIM_DURATION;
    value = usToTicks(value);  // convert to ticks after compensating for interrupt overhead - 12 Aug 2009    
    /*
    uint8_t oldSREG = SREG;
    cli();
    SREG = oldSREG;
    */

    servos[channel].ticks = value;
    //__check("Servo::writeMicroseconds(...) [ticks == %d]", value);
  }
}

// returns current pulse width as an angle between 0 and 180 degrees
int Servo::read()
{
  int value = map( this->readMicroseconds()+1, SERVO_MIN(), SERVO_MAX(), 0, 180);
  __check("Servo::read() [vlaue == %d]", value);
  return value;
}

// returns current pulse width in microseconds for this servo (was read_us() in first release)
int Servo::readMicroseconds()
{
  unsigned int pulsewidth;
  if( this->servoIndex != INVALID_SERVO )
    pulsewidth = ticksToUs(servos[this->servoIndex].ticks)  + TRIM_DURATION ;   // 12 aug 2009
  else
    pulsewidth  = 0;

  //__check("Servo::readMicroseconds() == %d]", pulsewidth);
  return pulsewidth;
}

// return true if this servo is attached, otherwise false 
bool Servo::attached()
{
  bool ia = servos[this->servoIndex].Pin.isActive;
  __check("Servo::attached() == %d", ia);
  return ia;
}
