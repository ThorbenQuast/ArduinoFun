#ifndef LED_H
#define LED_H
#include "../baseElement.h"
#include "Arduino.h"
#include "pitches.h"


int tones[] = {
  NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6};

class LEDs: public BaseElement {
    public: 
        LEDs(int, int, int);
        void onSetup();
        void onRun();
        void onIdle();
        void onWarning();
        void onError();
        void onCommand(COMMAND, int&);
    private:
        int pinLEDGreen;
        int pinLEDYellow;
        int pinSpeaker;
        void melody();
        void alarm_tone();      
        void stop_melody();      
        bool* melody_playing;  
        //values of the chosen melody   
        int toneindex;
        int increment;
        int duration;
};


LEDs::LEDs(int _pinLEDGreen, int _pinLEDYellow, int _pinSpeaker) {
    this->pinLEDGreen = _pinLEDGreen;
    this->pinLEDYellow = _pinLEDYellow;
    this->pinSpeaker = _pinSpeaker;
    melody_playing = new bool;
    *melody_playing = false;
    ownState=RUNNING;
}

void LEDs::onSetup(){
    pinMode(pinLEDGreen, OUTPUT);
    pinMode(pinLEDYellow, OUTPUT);
    
    digitalWrite(pinLEDGreen, HIGH);
    digitalWrite(pinLEDYellow, HIGH); 
    *melody_playing = true;
    for (int i=0; i<8; i++) {
        tone(pinSpeaker, tones[i], 50);
        delay(50);
    }
    *melody_playing = false;
}
void LEDs::onRun() {
    digitalWrite(pinLEDGreen, HIGH);
    digitalWrite(pinLEDYellow, LOW); 

    //play the default melody
    if (!*melody_playing) return;
    tone(pinSpeaker, tones[toneindex], duration);
    delay(duration);
    if (toneindex==7) increment=-1;
    if (toneindex==0) increment=+1;
    toneindex+=increment;    
};
void LEDs::onIdle() {
    digitalWrite(pinLEDGreen, LOW);
    digitalWrite(pinLEDYellow, LOW); 
    stop_melody();
};
void LEDs::onWarning() {
    digitalWrite(pinLEDGreen, LOW);
    digitalWrite(pinLEDYellow, HIGH); 
    stop_melody();
};
void LEDs::onError() {
    digitalWrite(pinLEDGreen, LOW);
    digitalWrite(pinLEDYellow, HIGH); 
    //alarm_tone();
};
void LEDs::onCommand(COMMAND cmd, int &val) {
    switch(cmd){
        case SPEAKER_ANNOY:
            melody(); break;
        case SPEAKER_STOP:
            stop_melody(); break;    
        default:
            break;
    }
};

void LEDs::melody(){
    toneindex=0;
    increment=1;
    duration = 500;
    *melody_playing = true;
}

void LEDs::alarm_tone(){
    tone(pinSpeaker, tones[7], 100);
    delay(100);    
}

void LEDs::stop_melody(){
    if (*melody_playing) tone(pinSpeaker, tones[0], 1);
    *melody_playing = false;
}

#endif

