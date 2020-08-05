//www.elegoo.com
//2016.12.9

int ledPin = 13;  // LED on Pin 13 of Arduino
int pirPin = 22; // Input for HC-S501

int pirValue; // Place to store read PIR Value


void setup() {
  
  pinMode(ledPin, OUTPUT);
  pinMode(pirPin, INPUT);
 
  digitalWrite(ledPin, LOW);
  
  
  
}

void loop() {
  pirValue = digitalRead(pirPin);
  digitalWrite(ledPin, pirValue);

  //value is high for ~8seconds after motion has ended

}
