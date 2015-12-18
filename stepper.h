#ifndef led_h
#define led_h

#include "Arduino.h"

/*
* The stepper motor has 3 relevant signals:
* - enable: allows stepping or not
* - direction: indicates rotation direction
* - step: commands a step
*
* The step signal is driven by Timer 3 with a constant square wave. 
* The stepper gets driven at 106.667 Hz to provide a 2 RPM rotation on the drive gear.
*/
// stepper I/O pin definitions (output)
const int ENABLE_STEPPER = 2;  // enable for SilentStepStick: HIGH = disabled; LOW = enabled
const int STEP_STEPPER = 5;    // step for SilentStepStick
const int DIR_STEPPER = 11;    // direction for SilentStepStick: HIGH = clockwise; LOW = anti-CW

// values to control PVM on pin 5 (the step signal)
// these get placed in TCCR3A
const int RUN = (1 << COM3A0) | (1 << WGM31) | (1 << WGM30);
const int NO_RUN = (0 << COM3A0) | (1 << WGM31) | (1 << WGM30);

// timer register OCR3A values to set the step frequency 
const unsigned int FREQ_NOMINAL = 9374u;   // 106.66 Hz with 8x prescale
const unsigned int FREQ_SET = 3124u;   // frequency is 24 x 106.666 Hz = 2560 Hz with 1x prescale 


/**************** functions **********************************/
void initClock(void); // set up all I/O and time keeping

void configNormal(void); // time keeping
void configForward(void); // forward setting
void configReverse(void); // reverse setting

void startClock(void); 
void stopClock(void);
void disable(void);

#endif
