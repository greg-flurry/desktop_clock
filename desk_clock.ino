/*
* This is the clock driving main, in cooperation with the other files in the sketch.
*
* In particular this supports a normal speed for keeping time, and a forward and reverse 
* setting speed for setting time. The expectation is that there is a push button switch
* that allows switching between the keeping and setting modes, as well as starting and
* stopping.
* 
* This file contains code for interpreting switch presses and releases to start and
* stop the clock (a single press/release), and setting a speed mode (a press of more
* than 2 seconds when the press/release results in stopping). When in a mode state, a pair 
* of press/release act as sort of a Morse code; 'dot/dot' means normal, 'dot/dash' means
* set forward, 'dash/dot' mean set reverse, 'dash/dash' is reserved.
*
*/ 
#include "stepper.h"

/* uncomment to debug clock; indicates time for a rotation of minute hand (millisec) */
//#define DEBUG_CLK 

/* switch related constants and variables */
const int buttonPin = 12;    // the pushbutton pin
const int DOT = 1;
const int DASH = 2;

boolean modeSet = false; // indicates not setting mode
int ddCount = 0; // count of dashes or dots
int mode = 0; // the running mode; can be
  // 0 = normal
  // 1 = forward set
  // 2 = reverse set
  // 3 = disable SSS
int dd[2]; // the dot/dash state of the two signals during mode set
int buttonState = HIGH;             // the current reading from the input pin
int lastButtonState = HIGH;   // the previous reading from the input pin

// timing variables
unsigned long pressTime = 0; // time of button press
unsigned long releaseTime = 0; // time of button release
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
unsigned long setDelay = 2000; // time for a press to indicate set
unsigned long dotDashBoundary = 1000; // time shorter = dot; time longer = dash

/* state of clock */
boolean running = false; // indicates the motor is running


/***********************************************
* SETUP
***********************************************/
void setup() {
  #ifdef DEBUG_CLK
  Serial.begin(19200);
  while (!Serial);
  Serial.println("Enabling clock.");
  #endif

  // set up input pin for switch
  pinMode(buttonPin, INPUT_PULLUP);
  
  // set up clock for normal operation
  initClock();

}

/***********************************************
* LOOP
***********************************************/
void loop() {
  // read the state of the switch 
  int reading = digitalRead(buttonPin);

  // check to see the button pressed/released and waited
  // long enough since the last change to ignore any noise

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debounce time
    lastDebounceTime = millis();
  }

  // if debounced
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;
      
      // if button press
      if (buttonState == LOW) { // button press
        #ifdef DEBUG_CLK
        Serial.println("Press");
        #endif
        pressTime = millis(); 

        // if not setting the mode
        if (modeSet == false) {
          if (running) {
            #ifdef DEBUG_CLK
            Serial.println("... stop");
            #endif
            // stop clock
            stopClock();
            running = false;
          } else { // not running
            #ifdef DEBUG_CLK
            Serial.println("... start");
            #endif
            // start clock
            startClock();
            running = true;
          }
        } else { // setting mode, so add to press count
          ddCount = ddCount + 1;
          #ifdef DEBUG_CLK
          Serial.println(ddCount);
          #endif
        }

      } else { // button release
        #ifdef DEBUG_CLK
        Serial.println("Release");
        #endif
        releaseTime = millis();
        // if stopped, then can consider entering mode setting phase
        if (!running) {
          // if not setting the mode
          if (modeSet == false) {
            // if want to set mode
            if ((releaseTime - pressTime) > setDelay) { 
              #ifdef DEBUG_CLK
              Serial.println("Set mode!");
              #endif
              modeSet = true;
              ddCount = 0;
              mode = 0;              
            }
          } else { // already setting mode
            // if get dot time
            if ((releaseTime - pressTime) < dotDashBoundary) {
              dd[ddCount-1] = DOT;
            } else {
              dd[ddCount-1] = DASH;
            }
            
            // if got both signals
            if (ddCount == 2) {
              // determine mode
              if (dd[0] == DASH) mode = mode + 2;
              if (dd[1] == DASH) mode = mode + 1;
              
              #ifdef DEBUG_CLK
              Serial.print("mode = ");
              Serial.println(mode);
              #endif
              switch (mode) {
                case 0:
                  configNormal();
                  break;
                case 1:
                  configForward();
                  break;
                case 2:
                  configReverse();
                  break;
                case 3:
                  disable();
                  break;
              }
                            
              // end mode setting
              modeSet = false;
            }    
          }
        }
      }
    }
  }

  // save the reading
  lastButtonState = reading;
    
}

