#ifndef CONFIG_H
#define CONFIG_H

#include "Arduino.h"

//pins for the engines
int PIN_LB = 12;     // define pin 12 
int PIN_LF = 3;    // define pin 3 
int PIN_RB = 13;    // define pin 13 
int PIN_RF = 11;    // define pin 11 


//pins for the LEDs
int PIN_LEDGreen = A2;
int PIN_LEDYellow = A3;
int PIN_Speaker = 10;


//gravity sensor
int PIN_BallSwitch = A0;
 
 

#define NELEMENTS 3
//hard coded indexes of the elements
enum ELEMENTS{
  //DHT11,
  LEDS,
  ENGINES,
  GRAVITYSENSOR
  //LIGHTSENSOR,
  //SERVO,
  //SPEAKER,
  //ULTRASONICSENSOR
};

#endif