#ifndef ENGINE_H
#define ENGINE_H
#include "../baseElement.h"
#include "Arduino.h"


class Engine: public BaseElement {
    public: 
        Engine(int, int, int, int);
        void onSetup();
        void onRun();
        void onIdle();
        void onWarning();
        void onError();
        void onCommand(COMMAND, int&);
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

        void proceed();
        void (Engine::*prev_command)();

        void setSpeed(int&);
        int speed;
};


Engine::Engine(int _pinLB, int _pinLF, int _pinRB, int _pinRF){
    this->pinLB = _pinLB;
    this->pinLF = _pinLF;
    this->pinRB = _pinRB;
    this->pinRF = _pinRF;

    prev_command=NULL;
    speed = 255;
   
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
void Engine::onWarning() {
    stopp();
}
void Engine::onError() {
    stopp();
}
void Engine::onCommand(COMMAND cmd, int &val) {    
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
        case DRIVE_PROCEED:
            this->proceed();
            break;
        case DRIVE_SETSPEED:
            this->setSpeed(val);
            break;
        default: break;
    }
}


void Engine::advance() {   
    digitalWrite(pinLB,LOW);  
    digitalWrite(pinRB,HIGH); 
    analogWrite(pinLF, this->speed);
    analogWrite(pinRF,this->speed);
    prev_command=&Engine::advance;  
    ownState=RUNNING;
}
void Engine::stopp() {        
    digitalWrite(pinLB,HIGH);
    digitalWrite(pinRB,HIGH);
    analogWrite(pinLF,0);
    analogWrite(pinRF,0); 
    prev_command=&Engine::stopp;
    ownState=IDLE;
}
void Engine::back() {        
    digitalWrite(pinLB,HIGH);  
    digitalWrite(pinRB,LOW); 
    analogWrite(pinLF, this->speed);
    analogWrite(pinRF,this->speed);   
    prev_command=&Engine::back;
    ownState=RUNNING;
}
void Engine::left() {    
    digitalWrite(pinLB,LOW);    
    digitalWrite(pinRB, LOW);  
    analogWrite(pinLF,this->speed);   
    analogWrite(pinRF,this->speed);
    ownState=RUNNING;
}
void Engine::right() {          
    digitalWrite(pinLB,HIGH);  
    digitalWrite(pinRB,HIGH);  
    analogWrite(pinLF,this->speed);  
    analogWrite(pinRF,this->speed);     
    ownState=RUNNING;
}

void Engine::proceed() {
    if (prev_command!=NULL) (this->*prev_command)();    
}


void Engine::setSpeed(int &speed) {
    this->speed = (255 - (9-speed)*15);
    proceed();
}



#endif

