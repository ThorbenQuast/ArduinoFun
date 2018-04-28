#ifndef BALLSWITCH_H
#define BALLSWITCH_H
#include "../baseElement.h"
#include "Arduino.h"


class BallSwitch: public BaseElement {
    public: 
        BallSwitch(int);
        void onSetup();
        void onRun();
        void onIdle();
        void onWarning();
        void onError();
        void onCommand(COMMAND, int&);
    private:
        int pinBS;
        int analogVal;
        int tiltCounter;
        int readValue();
        int threshold;    //need a threshold to count how often the tilt ball switch needs to be read as active in order to issue a warning
                          //the higher this value, the higher is the inertia.
};

BallSwitch::BallSwitch(int _pin) {
  this->pinBS = _pin;
  tiltCounter=0;
  threshold=50;
}

void BallSwitch::onCommand(COMMAND cmd, int &val) {}
void BallSwitch::onSetup() {
  pinMode(this->pinBS,INPUT);
  digitalWrite(this->pinBS, HIGH);
  tiltCounter=0;
  ownState=IDLE;
}

void BallSwitch::onRun() {
  if (readValue() > threshold) ownState=WARNING;
  else ownState=IDLE;
};
void BallSwitch::onIdle() {
  if (readValue() > threshold) ownState=WARNING;
  else ownState=IDLE;
};
void BallSwitch::onWarning() {
  if (readValue() > threshold) ownState=WARNING;
  else ownState=IDLE;
};
void BallSwitch::onError() {
  if (readValue() > threshold) ownState=WARNING;
  else ownState=IDLE;
};

int BallSwitch::readValue() {
  analogVal = digitalRead(pinBS);
  if (analogVal==1) {if (tiltCounter<2*threshold) tiltCounter++;}
  else tiltCounter=0;

  return tiltCounter;
}


#endif