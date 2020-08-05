//www.elegoo.com
//2016.12.12

/************************
Exercise the motor using
the L293D chip
************************/

#define ENABLE 10
#define DIRA 50
#define DIRB 52

int ledPins[4] = {45, 46, 47, 48};
int speedMap[5] = {0, 150, 180, 210, 255};
int statePin = 2;

int state=0;

void increaseState() {
  state = (state+1) % 5;  
}
 
void setup() {
  //---set pin direction
  pinMode(ENABLE,OUTPUT);
  pinMode(DIRA,OUTPUT);
  pinMode(DIRB,OUTPUT);

  for (short i=0; i<4; i++) pinMode(ledPins[i], OUTPUT);

  pinMode(statePin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(statePin), increaseState, FALLING);


}

void loop() {

  for (short i=0; i<4; i++) digitalWrite(ledPins[i], state>i ? HIGH : LOW);
  
  //---PWM example, full speed then slow
  if (state>0) {
    digitalWrite(DIRA,LOW); //one way
    digitalWrite(DIRB,HIGH); 
  }


  analogWrite(ENABLE,speedMap[state]); //enable on
}
   
