#ifndef SPEAKER_H
#define SPEAKER_H
#include "../baseElement.h"
#include "Arduino.h"
#include "pitches.h"


int available_tones[] = {
  NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B5, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6, NOTE_D6, NOTE_E6, NOTE_F6, NOTE_G6, NOTE_A6, NOTE_B6};


class Speaker: public BaseElement {
    public: 
        Speaker(int);
        void onSetup();
        void onRun();
        void onIdle();
        void onWarning();
        void onError();
        void onCommand(COMMAND, int&);
    private:
        int pinSpeaker;
        void melody(int);
        void alarm_tone();      
        void reset_melody();      
        void stop_melody();      
        bool* melody_playing;  
        //values of the chosen melody   
        
        int toneindex;
        int length;
        int* duration;
        int* tones;
        unsigned long T0;
};


Speaker::Speaker(int _pinSpeaker) {
    this->pinSpeaker = _pinSpeaker;
    melody_playing = new bool;
    *melody_playing = false;
    duration = NULL;
    tones = NULL;
    length = 0;
    toneindex = 0;
    T0 = 0;
    ownState=RUNNING;
}

void Speaker::onSetup(){
    *melody_playing = true;
    for (int i=0; i<21; i+=2) {
        tone(pinSpeaker, available_tones[i], 50);
        delay(50);
    }
    *melody_playing = false;
}
void Speaker::onRun() {
    
    //play the default melody
    if (!*melody_playing) return;
    
    unsigned long time = millis();

    if (T0==0) {
        T0=time;     //start
        tone(pinSpeaker, tones[toneindex], duration[toneindex]);
        return;
    }

    if (time-T0 > duration[toneindex]) {    //new tone
        toneindex = (toneindex+1) % length;
        tone(pinSpeaker, tones[toneindex], duration[toneindex]);
        T0 = time;
    }   
    
};
void Speaker::onIdle() {
    stop_melody();
};
void Speaker::onWarning() {
    stop_melody();
};
void Speaker::onError() {
    alarm_tone();
};
void Speaker::onCommand(COMMAND cmd, int &val) {
    switch(cmd){
        case SPEAKER_ANNOY:
            melody(1); break;
        case SPEAKER_STOP:
            stop_melody(); break;    
        default:
            break;
    }
};

void Speaker::melody(int index){
    reset_melody();

    if (index==1) {
        length = 10;
        duration = new int[length]; tones = new int[length];
        tones[0] = NOTE_D4; duration[0] = 500;
        tones[1] = NOTE_E4; duration[1] = 1000;
        tones[2] = NOTE_D4; duration[2] = 500;
        tones[3] = NOTE_A4; duration[3] = 600;
        tones[4] = NOTE_B5; duration[4] = 700;
        tones[5] = NOTE_C6; duration[5] = 800;
        tones[6] = NOTE_G5; duration[6] = 500;
        tones[7] = NOTE_D4; duration[7] = 200;
        tones[8] = NOTE_E5; duration[8] = 500;
        tones[9] = NOTE_D5; duration[9] = 1500;
        
    }

    *melody_playing = (length > 0);
}

void Speaker::alarm_tone(){
   
}

void Speaker::reset_melody() {
    if (duration != NULL) delete duration;
    if (tones != NULL) delete tones;
    length = 0;
    toneindex = 0;
    T0 = 0;       
}

void Speaker::stop_melody(){
    if (*melody_playing) tone(pinSpeaker, tones[0], 1);
    *melody_playing = false;
}

#endif

