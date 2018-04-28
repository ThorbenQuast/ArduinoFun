#ifndef BASEELEMENT_H
#define BASEELEMENT_H

#include <Arduino.h>
#include "SerialController.h"

enum STATE {
  SETUP,
  RUNNING,
  IDLE,
  WARNING,
  ERROR
};


//interface class
class BaseElement {
  public:
    virtual void onSetup() = 0;
    virtual void onRun() = 0;
    virtual void onIdle() = 0;
    virtual void onWarning() = 0;
    virtual void onError() = 0;
    virtual void onCommand(COMMAND, int&) = 0;

    STATE ownState;
    void enforceState(STATE s) {
      ownState = s;
    }
    STATE getState() {
      return ownState;
    }    

};


#endif