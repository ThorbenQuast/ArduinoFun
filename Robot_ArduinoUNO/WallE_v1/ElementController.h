#ifndef ELEMENTCONRTOLLER_H
#define ELEMENTCONRTOLLER_H

#include "baseElement.h"
#include "SerialController.h"
#include "config.h"




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
      for (int i=0; i<NELEMENTS; i++) elements[i]->enforceState(s);
      
    };
    
    void onLoop(){
      switch(globalState) {
        case SETUP:
          setupElements(); break;
        case RUNNING:
          runElements(); break;
        case IDLE:
          idleElements(); break;
        case WARNING:
          warningElements(); break;
        case ERROR:
          errorElements(); break;
        default:
          return;
      }
    }

    void updateGlobalState() {

      if (NRegistered != NELEMENTS) {globalState=ERROR; return;}      
      if (globalState==ERROR) return;   //error needs to be removed with a dedicated command

      for (int i=0; i<NELEMENTS; i++) {
        STATE elementState = elements[i]->getState();
        
        if (elementState==ERROR) {globalState=ERROR; return;}
        else if (elementState==WARNING) {globalState=WARNING;}
        if (elementState==WARNING) continue;    //could still be an error
        
        if (elementState==SETUP) globalState=SETUP;
        if ((elementState==RUNNING)&&(globalState!=SETUP)&&(globalState!=IDLE)) globalState= RUNNING; 
        if ((elementState==IDLE)&&(globalState!=SETUP)&&(globalState!=RUNNING)) globalState= IDLE; 
      }
    }
    

    void registerElement(BaseElement* element, ELEMENTS name) {
      elements[name] = element;
      NRegistered++;
    }

    void transferCommand(COMMAND cmd, int val) {
      if (globalState!=RUNNING) return;
      for (int i=0; i<NELEMENTS; i++) elements[i]->onCommand(cmd, val);
    }

  private:

    void setupElements(){for (int i=0; i<NELEMENTS; i++) elements[i]->onSetup();}
    void runElements(){for (int i=0; i<NELEMENTS; i++) elements[i]->onRun();}
    void idleElements(){for (int i=0; i<NELEMENTS; i++) elements[i]->onIdle();}
    void warningElements(){for (int i=0; i<NELEMENTS; i++) elements[i]->onWarning();}
    void errorElements(){for (int i=0; i<NELEMENTS; i++) elements[i]->onError();}
    
    BaseElement** elements;
    STATE globalState;
    int NRegistered;
};

#endif