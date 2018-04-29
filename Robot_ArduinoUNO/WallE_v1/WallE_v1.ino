//Author: Thorben Quast
//22 April 2018

//#include "config.h"
#include "ElementController.h"
//#include "SerialController.h"
//#include "baseElement.h"

//#include "elements/DHT11.h"
#include "elements/Engines.h"
#include "elements/GravitySensor.h"
#include "elements/LEDs.h"
//#include "elements/LightSensor.h"
//#include "elements/Servo.h"
#include "elements/Speaker.h"
//#include "elements/UltrasonicSensor.h"

char serial_val;
int receivingCommandState;

BaseElement* engineInstance = new Engine(PIN_LB, PIN_LF, PIN_RB, PIN_RF);
BaseElement* LEDsIntstance = new LEDs(PIN_LEDGreen, PIN_LEDYellow);
BaseElement* SpeakerIntstance = new Speaker(PIN_Speaker);
BaseElement* BallSwitchIntstance = new BallSwitch(PIN_BallSwitch);
ElementController eController;

void setup(){
  Serial.begin(9600);
  eController.registerElement(engineInstance, ENGINES);
  eController.registerElement(LEDsIntstance, LEDS);
  eController.registerElement(SpeakerIntstance, SPEAKER);
  eController.registerElement(BallSwitchIntstance, GRAVITYSENSOR);
  eController.forceGlobalState(SETUP);  
  eController.onLoop();
  eController.forceGlobalState(IDLE);
  receivingCommandState = 0;
}

    
void loop() {
  serial_val = Serial.read();
  if (!serialProcess()) return;	//more commands incoming
  eController.onLoop();
  eController.updateGlobalState(); 
}


bool serialProcess() {
  
  if (serial_val==-1) return true;
  Serial.print(receivingCommandState); Serial.print(": "); Serial.println(serial_val);

  switch(receivingCommandState) {
  	case 0:
		if(serial_val=='0') {
			eController.forceGlobalState(IDLE);
		}  	
		else if(serial_val=='1') {
			eController.forceGlobalState(eController.getGlobalState()==IDLE ? RUNNING : IDLE);
		}
		else if (serial_val=='D') {
			receivingCommandState=1000;
			return false;
		} else if (serial_val=='S') {
			receivingCommandState=2000;
			return false;
		}
		break;
	case 1000:
	  	if(serial_val=='F')eController.transferCommand(DRIVE_FORWARD, 0);
	  	if(serial_val=='B')eController.transferCommand(DRIVE_BACKWARD, 0);
	  	if(serial_val=='H')eController.transferCommand(DRIVE_STOP, 0);	  	
	  	if(serial_val=='L')eController.transferCommand(DRIVE_LEFT, 0);
	  	if(serial_val=='R')eController.transferCommand(DRIVE_RIGHT, 0);
	  	if(serial_val=='P')eController.transferCommand(DRIVE_PROCEED, 0);
	  	if(serial_val=='S') {receivingCommandState=1001; return false;}
	  	receivingCommandState=0;
	  	break;
	case 1001:	//speed adjustment
		if (serial_val - '0'==0) return false;
		eController.transferCommand(DRIVE_SETSPEED, serial_val - '0');
		receivingCommandState=0;
		break;
	case 2000:
		if(serial_val=='P')eController.transferCommand(SPEAKER_ANNOY, 0);
		if(serial_val=='M')eController.transferCommand(SPEAKER_STOP, 0);
		receivingCommandState=0;
		break;
  } 

  eController.updateGlobalState();
  return true;
}

