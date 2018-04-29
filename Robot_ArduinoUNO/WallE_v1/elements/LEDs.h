#ifndef LED_H
#define LED_H
#include "../baseElement.h"
#include "Arduino.h"

class LEDs: public BaseElement {
    public: 
        LEDs(int, int);
        void onSetup();
        void onRun();
        void onIdle();
        void onWarning();
        void onError();
        void onCommand(COMMAND, int&);
    private:
        int pinLEDGreen;
        int pinLEDYellow;
};


LEDs::LEDs(int _pinLEDGreen, int _pinLEDYellow) {
    this->pinLEDGreen = _pinLEDGreen;
    this->pinLEDYellow = _pinLEDYellow;
    ownState=RUNNING;
}

void LEDs::onSetup(){
    pinMode(pinLEDGreen, OUTPUT);
    pinMode(pinLEDYellow, OUTPUT);
    digitalWrite(pinLEDGreen, HIGH);
    digitalWrite(pinLEDYellow, HIGH); 
}
void LEDs::onRun() {
    digitalWrite(pinLEDGreen, HIGH);
    digitalWrite(pinLEDYellow, LOW); 
  
};
void LEDs::onIdle() {
    digitalWrite(pinLEDGreen, LOW);
    digitalWrite(pinLEDYellow, LOW); 
};
void LEDs::onWarning() {
    digitalWrite(pinLEDGreen, LOW);
    digitalWrite(pinLEDYellow, HIGH); 
};
void LEDs::onError() {
    digitalWrite(pinLEDGreen, LOW);
    digitalWrite(pinLEDYellow, HIGH); 
};
void LEDs::onCommand(COMMAND cmd, int &val) {
};


#endif

