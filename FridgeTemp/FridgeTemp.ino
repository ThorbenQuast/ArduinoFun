//Thorben Quast
//02 August 2020
//Simple Arduino MEGA2560 FW to control temperature of e.g. a fridge

//initialize DHT11 sensor
#include <SimpleDHT.h>
int pinDHT11 = 2;
SimpleDHT11 dht11;
byte temperature = 0;
byte humidity = 0;
byte data[40] ={ 0 };


// initialize lcd display
#include <LiquidCrystal.h>
int PIN_LCD_RS=7;
int PIN_LCD_E=8;
int PIN_LCD_D4=9;
int PIN_LCD_D5=10;
int PIN_LCD_D6=11;
int PIN_LCD_D7=12;
LiquidCrystal lcd(PIN_LCD_RS, PIN_LCD_E, PIN_LCD_D4, PIN_LCD_D5, PIN_LCD_D6, PIN_LCD_D7);

//define LEDs
int PIN_LED_RED = 26;
int PIN_LED_YELLOW = 32;
int PIN_LED_GREEN = 38;

//initialize sound maker
#include "pitches.h"
int PIN_SOUND=4;
// notes in the melody:
int melody[] ={
    NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_F4, NOTE_E4,
    NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_G4, NOTE_F4,
    NOTE_C4, NOTE_C4, NOTE_C5, NOTE_A4, NOTE_F4, NOTE_F4, NOTE_E4, NOTE_D4,
    NOTE_AS4, NOTE_AS4, NOTE_A4, NOTE_F4, NOTE_G4, NOTE_F4
};
int noteDurations[] ={
    4, 4, 2, 2, 2, 1, /*line 1*/
    4, 4, 2, 2, 2, 1, /*line 2*/
    4, 4, 2, 2, 4, 4, 2, 1, /*line 3*/
    4, 4, 2, 2, 2, 1 };

//max temp regulation 
int PIN_BUTTON = 3;
bool BTN_ALREADY_PRESSED;
int MAXTEMP = 0;
int maxTempCounter;
unsigned int iteration_counter;

//define statemachine
enum STATE {
    INIT,
    IDLE,
    GOODREADING,
    BADREADING,
    TEMPOK,
    TEMPWARNING
};
STATE current_state;

void setup() {
    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
    pinMode(PIN_LED_RED, OUTPUT);
    pinMode(PIN_LED_YELLOW, OUTPUT);
    pinMode(PIN_LED_GREEN, OUTPUT);
    pinMode(PIN_BUTTON, INPUT_PULLUP);
    BTN_ALREADY_PRESSED = false;

    current_state = INIT;
    signalState(current_state);
    iteration_counter = 0;
    maxTempCounter = 0;
    delay(2000);
    current_state = IDLE;
}

void signalState(STATE s) {
    switch (s) {
    case INIT:
        digitalWrite(PIN_LED_RED, HIGH);
        digitalWrite(PIN_LED_YELLOW, HIGH);
        digitalWrite(PIN_LED_GREEN, HIGH);
        break;
    case IDLE:
        digitalWrite(PIN_LED_RED, LOW);
        digitalWrite(PIN_LED_YELLOW, HIGH);
        digitalWrite(PIN_LED_GREEN, LOW);
        break;
    case GOODREADING:
        digitalWrite(PIN_LED_RED, LOW);
        digitalWrite(PIN_LED_YELLOW, HIGH);
        digitalWrite(PIN_LED_GREEN, HIGH);
        break;
    case BADREADING:
        digitalWrite(PIN_LED_RED, HIGH);
        digitalWrite(PIN_LED_YELLOW, HIGH);
        digitalWrite(PIN_LED_GREEN, LOW);
        break;
    case TEMPOK:
        digitalWrite(PIN_LED_RED, LOW);
        digitalWrite(PIN_LED_YELLOW, LOW);
        digitalWrite(PIN_LED_GREEN, HIGH);
        break;
    case TEMPWARNING:
        digitalWrite(PIN_LED_RED, HIGH);
        digitalWrite(PIN_LED_YELLOW, LOW);
        digitalWrite(PIN_LED_GREEN, LOW);
        break;
    }
}

void display_environemental_params(int temp, int humidity, bool good_reading) {
    lcd.setCursor(0, 1);
    lcd.print("               ");
    lcd.setCursor(0, 1);
    lcd.print(temp);
    lcd.setCursor(3, 1);
    lcd.print("C");
    if (good_reading) {
        lcd.setCursor(5, 1);
        lcd.print(" - ");
        lcd.setCursor(8, 1);
        lcd.print(humidity);
        lcd.setCursor(11, 1);
        lcd.print("%");
        lcd.setCursor(12, 1);
        lcd.print("     ");
    }
    else {
        lcd.setCursor(5, 1);
        lcd.print("  (last)    ");
    }
}

void loop() {
    delay(50);
    if (digitalRead(PIN_BUTTON) == LOW) {
        if (!BTN_ALREADY_PRESSED) MAXTEMP = (MAXTEMP+1) % 20;
        BTN_ALREADY_PRESSED = true;
        current_state=IDLE;     //reset all potential errors
        iteration_counter=1;      //reset the iteration counter
    }
    else {
        BTN_ALREADY_PRESSED = false;
    }

    lcd.setCursor(0, 0);
    lcd.print("Temp. - Hum.     ");
    lcd.setCursor(14, 0);
    if (MAXTEMP>0) lcd.print(MAXTEMP);
    else lcd.print("- ");
    signalState(current_state);

    if (current_state==TEMPWARNING) {
        for (int thisNote = 0; thisNote < 11; thisNote++) {
            tone(PIN_SOUND, melody[thisNote], 1000/noteDurations[thisNote]);
            delay(noteDurations[thisNote]*1.3);
        }
        iteration_counter=0;
    }
    else if (current_state==GOODREADING) {
        display_environemental_params(temperature, humidity, true);
        if ((MAXTEMP>0)&&((int)temperature>=MAXTEMP)) maxTempCounter++;
        else maxTempCounter=0;

        if (maxTempCounter>=3) current_state = TEMPWARNING;
        else current_state = TEMPOK;
        return;
    }
    else if (current_state==BADREADING) {
        display_environemental_params(temperature, humidity, false);
        current_state=IDLE;
        return;
    }

    if (iteration_counter%30==0) {
        if (!dht11.read(pinDHT11, &temperature, &humidity, data)) {
            //good reading
            current_state = GOODREADING;
        }
        else {
            current_state = BADREADING;
        }
    }

    iteration_counter++;
}
