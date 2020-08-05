//www.elegoo.com
//04 March 2018
//Thorben Quast

#include "IRremote.h"

int receiver = 22; // Signal Pin of IR receiver to Arduino Digital Pin 11
int LEDPin = 13;
bool onstate = false;
int remoteID = 1;   //own Hannspree remote

#include "pitches.h"
// notes in the melody:
int melody[] = {
  NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6, NOTE_D6, NOTE_E6};
int duration = 1000;  // 1000 miliseconds
int musicPin = 8;
int BLUEPin = 2;
int GREENPin = 3;
int REDPin = 4;
int LEDColour = -1;


#include "LedControl.h"
LedControl lc=LedControl(32,30,31,1);
void turnOffMatrixLED(){
  lc.clearDisplay(0);
}
void showT(){
  lc.clearDisplay(0);
  lc.setColumn(0,7,B11111111);
  lc.setColumn(0,6,B11111111);
  lc.setRow(0, 3, B11111111);
  lc.setRow(0, 4, B11111111);
}
void showI(){
  lc.clearDisplay(0);
  lc.setRow(0, 3, B11111111);
  lc.setRow(0, 4, B11111111);
}
void showA(){
  lc.clearDisplay(0);
  lc.setColumn(0, 7,B00011000);
  lc.setColumn(0, 6, B00111100);
  lc.setColumn(0, 5, B01100110);
  lc.setColumn(0, 4, B01100110);
  lc.setColumn(0, 3, B11111111);
  lc.setColumn(0, 2, B11000011);
  lc.setColumn(0, 1, B11000011);
  lc.setColumn(0, 0, B11000011);
}
void showM(){
  lc.clearDisplay(0);
  for (int j=7; j>=5; j--) lc.setColumn(0, j, B01100110);
  lc.setColumn(0, 4, B11111111);
  for (int j=3; j>=0; j--) lc.setColumn(0, j, B11011011);
}
void showO(){
  lc.clearDisplay(0);
  lc.setColumn(0, 7, B00111100);
  lc.setColumn(0, 6, B01111110);
  for (int i=5; i>=2; i--) lc.setColumn(0, i, B11000011);
  lc.setColumn(0, 1, B01111110);
  lc.setColumn(0, 0, B00111100);
}
void showExclPoint(){
  lc.clearDisplay(0);
  for (int j=7; j>=4; j--) lc.setColumn(0, j, B00011000);
  for (int j=1; j>=0; j--) lc.setColumn(0, j, B00011000);
}

void printMessage(int state) {
  int option = (unsigned)state%7;
  switch(option) {
    case 0: turnOffMatrixLED(); break;    //none
    case 1: showT(); break;    //T
    case 2: showI(); break;    //I
    case 3: showA(); break;    //A
    case 4: showM(); break;    //M
    case 5: showO(); break;    //O
    case 6: showExclPoint(); break;      //excl. point
    }    
}
int messageState = 0;
void GreetBellissima() {
  lightLED(-1);
  for(int i=0; i<7; i++) {
    messageState=i;
    printMessage(messageState);
    delay(500);
  }
  messageState=0;
  printMessage(messageState);
  LEDColour = 10;
}



/*-----( Declare objects )-----*/
IRrecv irrecv(receiver);     // create instance of 'irrecv'
decode_results results;      // create instance of 'decode_results'

void lightLED(int state){
  short valueBlue = 0, valueGreen = 0, valueRed = 0; 
  
  switch(state) {
    case 9: valueBlue=255; valueGreen=255; valueRed=255; break;
    case 8: valueBlue=255; valueGreen=0; valueRed=255; break;
    case 7: valueBlue=255; valueGreen=0; valueRed=0; break;
    case 6: valueBlue=110; valueGreen=110; valueRed=0; break;
    case 5: valueBlue=50; valueGreen=180; valueRed=0; break;
    case 4: valueBlue=0; valueGreen=255; valueRed=0; break;
    case 3: valueBlue=0; valueGreen=180; valueRed=50; break;
    case 2: valueBlue=0; valueGreen=110; valueRed=110; break;
    case 1: valueBlue=0; valueGreen=50; valueRed=180; break;
    case 0: valueBlue=0; valueGreen=0; valueRed=255; break;
    case 10: valueBlue=0; valueGreen=0; valueRed=10; break;
    default: break;
    
    }
  analogWrite(BLUEPin, valueBlue);
  analogWrite(GREENPin, valueGreen);
  analogWrite(REDPin, valueRed);
  }

/*-----( Function )-----*/
void translateIR() // takes action based on IR code received

// describing Remote IR codes 

{
  if (remoteID==1) {
    if(results.value==0xCC33C13E) {
      onstate = !onstate;
      LEDColour = onstate ? 10 : -1;
      messageState = 0;
      Serial.println("POWER"); 
    }
    if (!onstate) return;
    switch(results.value){
      case 0xCC33E11E: Serial.println("VOL+"); break;
      case 0xCC33D12E: Serial.println("VOL-");    break;
      case 0xCC33F10E: Serial.println("NEXT"); messageState++; printMessage(messageState);   break;
      case 0xCC33C936: Serial.println("PREV"); messageState--; printMessage(messageState);   break;
      case 0xCC334BB4: Serial.println("OK");  GreetBellissima();  break;
      case 0xCC3309F6: Serial.println("LEFT");    break;
      case 0xCC3349B6: Serial.println("RIGHT");    break;
      case 0xCC3359A6: Serial.println("UP");    break;
      case 0xCC33D926: Serial.println("DOWN");    break;
      case 0xCC338976: Serial.println("MENU");    break;
      case 0xCC3329D6: Serial.println("INFO");    break;
      case 0xCC3331CE: Serial.println("0"); tone(musicPin, melody[0], duration); LEDColour=0;  break;
      case 0xCC3301FE: Serial.println("1"); tone(musicPin, melody[1], duration); LEDColour=1;  break;
      case 0xCC33817E: Serial.println("2"); tone(musicPin, melody[2], duration); LEDColour=2;  break;
      case 0xCC3341BE: Serial.println("3"); tone(musicPin, melody[3], duration); LEDColour=3;  break;
      case 0xCC3321DE: Serial.println("4"); tone(musicPin, melody[4], duration); LEDColour=4;  break;
      case 0xCC33A15E: Serial.println("5"); tone(musicPin, melody[5], duration); LEDColour=5;  break;
      case 0xCC33619E: Serial.println("6"); tone(musicPin, melody[6], duration); LEDColour=6;  break;
      case 0xCC3311EE: Serial.println("7"); tone(musicPin, melody[7], duration); LEDColour=7;  break;
      case 0xCC33916E: Serial.println("8"); tone(musicPin, melody[8], duration); LEDColour=8;  break;
      case 0xCC3351AE: Serial.println("9"); tone(musicPin, melody[9], duration); LEDColour=9;  break;
      
      default:
        break;
      
    }// End Case
    delay(1);
  }

  


} //END translateIR
void setup()   /*----( SETUP: RUNS ONCE )----*/
{
  Serial.begin(9600);
  Serial.println("IR Receiver Button Decode"); 
  irrecv.enableIRIn(); // Start the receiver
  pinMode(LEDPin, OUTPUT);
  onstate = false;
  pinMode(BLUEPin, OUTPUT);
  pinMode(GREENPin, OUTPUT);
  pinMode(REDPin, OUTPUT);
  lightLED(LEDColour);

  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,8);
  /* and clear the display */
  turnOffMatrixLED();


}/*--(end setup )---*/


void loop()   /*----( LOOP: RUNS CONSTANTLY )----*/
{
  
  if (irrecv.decode(&results)) // have we received an IR signal?
  { 
    translateIR(); 
    irrecv.resume(); // receive the next value
  }  
  digitalWrite(LEDPin, onstate ? HIGH: LOW);
  lightLED(LEDColour);
  if (!onstate) {printMessage(messageState); return;}
  
}/* --(end main loop )-- */


