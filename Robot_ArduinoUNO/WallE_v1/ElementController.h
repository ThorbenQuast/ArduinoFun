#ifndef ELEMENTCONRTOLLER_H
#define ELEMENTCONRTOLLER_H

#include "baseElement.h"
#include "SerialController.h"

//hard coded indexes of the elements
enum ELEMENTS{
  //DHT11,
  ENGINES,
  //GRAVITYSENSOR,
  LEDS
  //LIGHTSENSOR,
  //SERVO,
  //SPEAKER,
  //ULTRASONICSENSOR
};
#define NELEMENTS 2


class ElementController {
  public:
    ElementController(){
      NRegistered=0;
      elements = new BaseElement*[NELEMENTS];
    };
    ~ElementController(){};
    STATE getGlobalState() {return globalState;};
    void forceGlobalState(STATE s) {
      globalState=s;
      for (int i=0; i<NELEMENTS; i++) {
        elements[i]->enforceState(s);
        elements[i]->eraseCommandForTransmission();
      }
    };
    
    void onLoop(){
      switch(globalState) {
        case SETUP:
          setupElements(); break;
        case RUNNING:
          runElements(); break;
        case IDLE:
          idleElements(); break;
        case ERROR:
          errorElements(); break;
        default:
          return;
      }
    }

    void updateGlobalState() {
      if (NRegistered != NELEMENTS) {globalState=ERROR; return;}      
      for (int i=0; i<NELEMENTS; i++) {
        STATE elementState = elements[i]->getState();
        if (elementState==ERROR) {globalState=ERROR; return;}
        else if (elementState==SETUP) globalState=SETUP;
        else if ((elementState==RUNNING)&&(globalState!=SETUP)&&(globalState!=IDLE)) globalState= RUNNING; 
      }
    }
    
    void distributeElementCommands() {
      for (int i=0; i<NELEMENTS; i++) {
        COMMAND elementCommand = elements[i]->transmitCommand();
        if (elementCommand != EMPTYCOMMAND) {
          transferCommand(elementCommand);
        }

      }
    }

    void registerElement(BaseElement* element, ELEMENTS name) {
      elements[name] = element;
      NRegistered++;
    }

    void transferCommand(COMMAND cmd) {
      if (globalState!=RUNNING) return;
      for (int i=0; i<NELEMENTS; i++) elements[i]->onCommand(cmd);
    }

  private:

    void setupElements(){for (int i=0; i<NELEMENTS; i++) elements[i]->onSetup();}
    void runElements(){for (int i=0; i<NELEMENTS; i++) elements[i]->onRun();}
    void idleElements(){for (int i=0; i<NELEMENTS; i++) elements[i]->onIdle();}
    void errorElements(){for (int i=0; i<NELEMENTS; i++) elements[i]->onError();}
    
    BaseElement** elements;
    STATE globalState;
    int NRegistered;
};

#endif