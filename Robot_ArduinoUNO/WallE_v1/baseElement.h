#ifndef BASEELEMENT_H
#define BASEELEMENT_H

#include <Arduino.h>
#include "SerialController.h"

enum STATE {
  SETUP,
  RUNNING,
  IDLE,
  ERROR
};


//interface class
class BaseElement {
  public:
    virtual void onSetup() = 0;
    virtual void onRun() = 0;
    virtual void onIdle() = 0;
    virtual void onError() = 0;
    virtual void onCommand(COMMAND) = 0;

    STATE ownState;
    void enforceState(STATE s) {
      ownState = s;
    }
    STATE getState() {
      return ownState;
    }    

    COMMAND commandForTransmission;
    COMMAND transmitCommand() {
      COMMAND tbr = commandForTransmission;
      eraseCommandForTransmission();
      return tbr;
    }
    void eraseCommandForTransmission() {commandForTransmission=EMPTYCOMMAND;}
};


#endif