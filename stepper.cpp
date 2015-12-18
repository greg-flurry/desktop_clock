/*
* This contains code for controlling the SilentStepStick that drives the stepper motor.
*/
#include "stepper.h"

/* leave this definition to debug motor control */
//#define DEBUG_MOTOR

/*
* initClock
*/
void initClock() {
  #ifdef DEBUG_MOTOR
  Serial.println("... initClock");
  #endif
  // set up I/O
  pinMode(STEP_STEPPER, OUTPUT); // step pin
  pinMode(DIR_STEPPER, OUTPUT); // direction pin
  pinMode(ENABLE_STEPPER, OUTPUT); // enable pin
  
  // set initial state to time keeping (normal speed) and STOPPED
  digitalWrite(ENABLE_STEPPER, HIGH); // disable
  configNormal();
  digitalWrite(ENABLE_STEPPER, LOW); // enable
}

/*
* startClock
*/
void startClock() {
  #ifdef DEBUG_MOTOR
  Serial.println("... startClock");
  #endif
  digitalWrite(ENABLE_STEPPER, LOW); // enable, in case previously disabled
  TCCR3A = RUN;
}

/*
* stopClock
*/
void stopClock() {
  #ifdef DEBUG_MOTOR
  Serial.println("... stopClock");
  #endif
  TCCR3A = NO_RUN;
}

/*
* disable
*/
void disable() {
  #ifdef DEBUG_MOTOR
  Serial.println("... disable");
  #endif
  TCCR3A = NO_RUN; // turn off step signal
  digitalWrite(ENABLE_STEPPER, HIGH); // disable
}

/*
* configNormal
*/
void configNormal() {
  #ifdef DEBUG_MOTOR
  Serial.println("... configNormal");
  #endif
  // go clockwise
  digitalWrite(DIR_STEPPER, HIGH); // clockwise
  
  // Configure timer 3 for Fast PWM mode via OCR3A setting TOP;
  // this is mode 15 WGM3:0 = 1111
  // use 8x prescaling; this is CS2:0 = 010
  // use the "special" square wave mode, see end of 14.8.3 in data sheet;
  // it is "toggle" with COM3A1:0 = 01

  TCCR3A = NO_RUN;
  TCCR3B = (1 << WGM33) | (1 << WGM32) | (1 << CS31); // 8x prescale

  // set the frequency so that we get a 106.666 Hz square wave
//  const unsigned int val = FREQ_NOMINAL;   // frequency is 106.666 Hz with 8x prescale 
  OCR3AH = FREQ_NOMINAL >> 8;    
  OCR3AL = FREQ_NOMINAL & 0x00ff;     
  
}

/*
* configForward
*/
void configForward() {
  #ifdef DEBUG_MOTOR
  Serial.println("... configForward");
  #endif
  // go clockwise
  digitalWrite(DIR_STEPPER, HIGH); // clockwise
  
  // Configure timer 3 for Fast PWM mode via OCR3A setting TOP;
  // this is mode 15 WGM3:0 = 1111
  // use 1x prescaling; this is CS2:0 = 001
  // use the "special" square wave mode, see end of 14.8.3 in data sheet;
  // it is "toggle" with COM3A1:0 = 01

  TCCR3A = NO_RUN;
  TCCR3B = (1 << WGM33) | (1 << WGM32) | (1 << CS30); // 1x prescale

  // set the frequency so that we get a 24 x 106.666 Hz square wave
  OCR3AH = FREQ_SET >> 8;    
  OCR3AL = FREQ_SET & 0x00ff;     
  
}

/*
* configReverse
*/
void configReverse() {
  #ifdef DEBUG_MOTOR
  Serial.println("... configReverse");
  #endif
  // go anit-clockwise
  digitalWrite(DIR_STEPPER, LOW); // anti-clockwise
  
  // Configure timer 3 for Fast PWM mode via OCR3A setting TOP;
  // this is mode 15 WGM3:0 = 1111
  // use 1x prescaling; this is CS2:0 = 001
  // use the "special" square wave mode, see end of 14.8.3 in data sheet;
  // it is "toggle" with COM3A1:0 = 01

  TCCR3A = NO_RUN;
  TCCR3B = (1 << WGM33) | (1 << WGM32) | (1 << CS30); // 1x prescale

  // set the frequency so that we get a 24 x 106.666 Hz square wave
  OCR3AH = FREQ_SET >> 8;    
  OCR3AL = FREQ_SET & 0x00ff;     
  
}

