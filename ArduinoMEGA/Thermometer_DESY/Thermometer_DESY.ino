//www.elegoo.com
//2016.12.9

#include <LiquidCrystal.h>
#include <SimpleDHT.h>
int tempPin = 0;
int pinDHT11 = 24;
SimpleDHT11 dht11;

int redPin=36;
int greenPin=38;
int yellowPin=40;

//                BS  E  D4 D5  D6 D7
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

unsigned int loopCount;
unsigned int PERIOD=60;

void setup()
{
  lcd.begin(16, 2);
  Serial.begin(19200);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, HIGH);
  digitalWrite(yellowPin, LOW);
  
  loopCount=0;
}
void loop() { //temperature reading from the resistor
  int tempReading = analogRead(tempPin);
  // This is OK
  double tempK = log(10000.0 * ((1024.0 / tempReading - 1)));
  tempK = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempK * tempK )) * tempK );       //  Temp Kelvin
  float tempC = tempK - 273.15;            // Convert Kelvin to Celcius
  
  
  //temperature and humidity from dht11
  byte temperature = 0;
  byte humidity = 0;
  byte data[40] = {0};
  while (dht11.read(pinDHT11, &temperature, &humidity, data)) {
    delay(10);
    digitalWrite(redPin, HIGH);
    digitalWrite(greenPin, LOW);
  }
  
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, HIGH);
  
  // Display Temperature in C
  lcd.setCursor(0, 0);
  lcd.print("   (     ) deg C ");
  lcd.setCursor(0, 0);
  lcd.print((int)temperature);
  // Display Temperature in F
  lcd.setCursor(4, 0);
  // Display Temperature in C
  lcd.print(tempC);
  lcd.setCursor(0, 1);
  lcd.print("   %            ");
  lcd.setCursor(0, 1);
  lcd.print((int)humidity);
  lcd.setCursor(13, 1);
  lcd.print(loopCount%PERIOD);

  if (loopCount%PERIOD>PERIOD-3) digitalWrite(yellowPin, HIGH);
  else digitalWrite(yellowPin, LOW);
  
  if (loopCount%60==0) {
    Serial.print((float)tempC);
    Serial.print(",");
    Serial.print((int)temperature);
    Serial.print(",");
    Serial.println((int)humidity);
  } else Serial.println("XX.XX,XX,XX");

  delay(1000);
  loopCount++;
}
