#ifndef ENGINE_H
#define ENGINE_H
#include "../baseElement.h"
#include "Arduino.h"


class Engine: public BaseElement {
    public: 
        Engine(int pinLB, int pinLF, int pinRB, int pinRF);
        void onSetup();
        void onRun();
        void onIdle();
        void onError();
        void onCommand(COMMAND cmd);
    private:
        int pinLB;
        int pinLF;
        int pinRB;
        int pinRF;        

        void advance();
        void stopp();
        void left();
        void right();        
        void back();
};


Engine::Engine(int _pinLB, int _pinLF, int _pinRB, int _pinRF){
    this->pinLB = _pinLB;
    this->pinLF = _pinLF;
    this->pinRB = _pinRB;
    this->pinRF = _pinRF;
   
}
void Engine::onSetup() {
    ownState=IDLE;
    pinMode(pinLB, OUTPUT); 
    pinMode(pinLF, OUTPUT); 
    pinMode(pinRB, OUTPUT); 
    pinMode(pinRF, OUTPUT); 
}
void Engine::onRun() {}
void Engine::onIdle() {
    stopp();
}
void Engine::onError() {
    stopp();
}
void Engine::onCommand(COMMAND cmd) {    
    switch (cmd) {
        case DRIVE_FORWARD:
            this->advance();
            break;
        case DRIVE_BACKWARD:
            this->back();
            break;
        case DRIVE_LEFT:
            this->left();
            break;
        case DRIVE_RIGHT:
            this->right();
            break;
        case DRIVE_STOP:
            this->stopp();
            break;
        default: break;
    }
}


void Engine::advance() {   
    digitalWrite(pinLB,LOW);  
    digitalWrite(pinRB,HIGH); 
    analogWrite(pinLF, 255);
    analogWrite(pinRF,255);  
    ownState=RUNNING;
}
void Engine::stopp() {        
    digitalWrite(pinLB,HIGH);
    digitalWrite(pinRB,HIGH);
    analogWrite(pinLF,0);
    analogWrite(pinRF,0); 
    ownState=IDLE;
}
void Engine::back() {        
    digitalWrite(pinLB,HIGH);  
    digitalWrite(pinRB,LOW); 
    analogWrite(pinLF, 255);
    analogWrite(pinRF,255);   
    ownState=RUNNING;
}
void Engine::left() {    
    digitalWrite(pinLB,LOW);    
    digitalWrite(pinRB, LOW);  
    analogWrite(pinLF,255);   
    analogWrite(pinRF,255);
    ownState=RUNNING;
}
void Engine::right() {          
    digitalWrite(pinLB,HIGH);  
    digitalWrite(pinRB,HIGH);  
    analogWrite(pinLF,255);  
    analogWrite(pinRF,255);     
    ownState=RUNNING;
}





#endif

