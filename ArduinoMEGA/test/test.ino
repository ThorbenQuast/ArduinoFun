//www.elegoo.com
//2018.03.01

//blinking LED
#define LED_PIN 13
double T0;
double dT;

//temperature readout
#define DHT11_PIN 24
#include <SimpleDHT.h>
SimpleDHT11 dht11;


void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  T0 = millis();

  Serial.begin(9600);
}

void measureTemperature() { 
  // read with raw sample data.
  byte temperature = 0;
  byte humidity = 0;
  byte data[40] = {0};
  while (dht11.read(DHT11_PIN, &temperature, &humidity, data)) continue;
  
  Serial.print(temperature);
}


void loop() {
  //make the LED blink
  dT = millis() - T0;
  if(dT > 1000) {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    T0 += dT;
  }
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    // add it to the inputString:
    if ((String)inChar=="T") measureTemperature();
    
  }
}
