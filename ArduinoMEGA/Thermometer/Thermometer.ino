//www.elegoo.com
//2016.12.9

#include <LiquidCrystal.h>
int tempPin = 15;
int onOffPin = 2;
int displayLightPin = 40;
bool onState;
bool lcdOnState;
short showGreeting;
//                BS  E  D4 D5  D6 D7
LiquidCrystal lcd(23, 25, 27, 29, 31, 33);



void toggleOnOffState() {
  onState = !onState;
  }
void activateLCDDisplay(bool on) {
  if (on) {
    lcd.display();
    lcdOnState=true;
    showGreeting=0;
  }
  else {
    lcd.noDisplay();
    lcdOnState=false;
  }
}

void setup()
{
  lcd.begin(16, 2);
  onState=true;
  showGreeting=0;
  pinMode(13, OUTPUT);
  pinMode(displayLightPin, OUTPUT);
  pinMode(onOffPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(onOffPin), toggleOnOffState, FALLING);
}

void loop()
{ 
  //control LEDs
  digitalWrite(13, !onState ? HIGH: LOW);
  digitalWrite(displayLightPin, onState ? HIGH: LOW);
  
  if (onState!=lcdOnState){
    activateLCDDisplay(onState);  
  }
  if (!onState) return;
  int tempReading = analogRead(tempPin);
  // This is OK
  double tempK = log(10000.0 * ((1024.0 / tempReading - 1)));
  tempK = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempK * tempK )) * tempK );       //  Temp Kelvin
  float tempC = tempK - 273.15;            // Convert Kelvin to Celcius
  
  
  lcd.setCursor(0, 0);
  if (showGreeting/4==0) lcd.print("Ciao amore !    ");
  else if (showGreeting/4==1) {
    if (tempC < 19) lcd.print("Fa freddo :(    ");
    else if(tempC > 25) lcd.print("E tropo caldo :(");
    else lcd.print("Fa ottimo :)    ");
  } 
  else if (showGreeting/4==2) lcd.print("E sei bella!    ");
  else  lcd.print("Ft ft.          ");
  
  // Display Temperature in C
  lcd.setCursor(0, 1);
  lcd.print("Temp         C  ");
  // Display Temperature in F
  //lcd.print("Temp         F  ");
  lcd.setCursor(6, 1);
  // Display Temperature in C
  lcd.print(tempC);
  
  showGreeting = (showGreeting+1) % 16;
  delay(500);
  
}
