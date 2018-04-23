//Author: Thorben Quast
//22 April 2018

#include "baseElement.h"
#include "config.h"
#include "ElementController.h"
#include "SerialController.h"

//#include "elements/DHT11.h"
#include "elements/Engines.h"
//#include "elements/GravitySensor.h"
#include "elements/LEDs.h"
//#include "elements/LightSensor.h"
//#include "elements/Servo.h"
//#include "elements/Speaker.h"
//#include "elements/UltrasonicSensor.h"

char serial_val;

BaseElement* engineInstance = new Engine(PIN_LB, PIN_LF, PIN_RB, PIN_RF);
BaseElement* LEDsIntstance = new LEDs(PIN_LEDGreen, PIN_LEDYellow, PIN_Speaker);
ElementController eController;

void setup(){
  Serial.begin(9600);
  eController.registerElement(engineInstance, ENGINES);
  eController.registerElement(LEDsIntstance, LEDS);
  eController.forceGlobalState(SETUP);  
  eController.onLoop();
  eController.forceGlobalState(IDLE);
}

    
void loop() {
  serial_val = Serial.read();
  serialProcess(serial_val);
  eController.onLoop();
  eController.distributeElementCommands();
  eController.updateGlobalState(); 
}


void serialProcess(char &val) {
  
  if (val==-1) return;

  //this has to be done within the SerialController.h file
  if(val=='U')eController.transferCommand(DRIVE_FORWARD);
  if(val=='D')eController.transferCommand(DRIVE_BACKWARD);
  if(val=='L')eController.transferCommand(DRIVE_LEFT);
  if(val=='R')eController.transferCommand(DRIVE_RIGHT);
  if(val=='S')eController.transferCommand(DRIVE_STOP);
  if(val=='M')eController.transferCommand(SPEAKER_ANNOY);
  if(val=='N')eController.transferCommand(SPEAKER_STOP);
  

  if(val=='0')eController.forceGlobalState(eController.getGlobalState()==IDLE ? RUNNING : IDLE);

  eController.updateGlobalState();
}

