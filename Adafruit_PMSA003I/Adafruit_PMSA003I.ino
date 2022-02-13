//Thorben Quast
//13 February 2022
//Particle rate count test for HGC Si clean room

#include "Adafruit_PM25AQI.h"
#include <SimpleDHT.h>
#include <LiquidCrystal.h>

// initialize lcd display
#include <LiquidCrystal.h>
int PIN_LCD_RS=7;
int PIN_LCD_E=8;
int PIN_LCD_D4=9;
int PIN_LCD_D5=10;
int PIN_LCD_D6=11;
int PIN_LCD_D7=12;
LiquidCrystal lcd(PIN_LCD_RS, PIN_LCD_E, PIN_LCD_D4, PIN_LCD_D5, PIN_LCD_D6, PIN_LCD_D7);

int PIN_BUTTON = 3;

//define LEDs
int PIN_LED_RED = 26;
int PIN_LED_YELLOW = 32;
int PIN_LED_GREEN = 38;

int loop_counter = 0;
int particle_type = 0;

//define statemachine
enum STATE {
    INIT,
    IDLE,
    GOODREADING,
    BADREADING
};
STATE current_state;

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
        digitalWrite(PIN_LED_YELLOW, LOW);
        digitalWrite(PIN_LED_GREEN, HIGH);
        break;
    case BADREADING:
        digitalWrite(PIN_LED_RED, HIGH);
        digitalWrite(PIN_LED_YELLOW, HIGH);
        digitalWrite(PIN_LED_GREEN, LOW);
        break;
    }
}

void display_particle_count(float psize, int pcount) {
    lcd.setCursor(0, 0);
    lcd.print(">                  ");
    lcd.setCursor(1, 0);
    lcd.print(psize);
    lcd.setCursor(6, 0);
    lcd.print("mum:");
    lcd.setCursor(0, 1);
    lcd.print("            ");
    lcd.setCursor(0, 1);
    lcd.print(pcount);
    lcd.print(" / 0.1L");

};

// If your PM2.5 is UART only, for UNO and others (without hardware serial) 
// we must use software serial...
// pin #2 is IN from sensor (TX pin on sensor), leave pin #3 disconnected
// comment these two lines if using hardware serial
//#include <SoftwareSerial.h>
//SoftwareSerial pmSerial(2, 3);

Adafruit_PM25AQI aqi = Adafruit_PM25AQI();

void setup() {
  loop_counter = 0;
  current_state =INIT;
  signalState(current_state);  
  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_LED_YELLOW, OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT);  
  pinMode(PIN_BUTTON, INPUT_PULLUP);

  lcd.begin(16, 2);


  // Wait for serial monitor to open
  //Serial.begin(115200);
  //while (!Serial) delay(10);

  //Serial.println("Adafruit PMSA003I Air Quality Sensor");

  // Wait one second for sensor to boot up!
  delay(1000);

  // If using serial, initialize it and set baudrate before starting!
  // Uncomment one of the following
  //Serial1.begin(9600);
  //pmSerial.begin(9600);

  // There are 3 options for connectivity!
  if (! aqi.begin_I2C()) {      // connect to the sensor over I2C
  //if (! aqi.begin_UART(&Serial1)) { // connect to the sensor over hardware serial
  //if (! aqi.begin_UART(&pmSerial)) { // connect to the sensor over software serial 
    //Serial.println("Could not find PM 2.5 sensor!");
    while (1) delay(10);
  }
  current_state =IDLE;
  signalState(current_state);
  //Serial.println("PM25 found!");
}

void loop() {
  delay(10);
  loop_counter = (loop_counter + 1) % 100;
  if (digitalRead(PIN_BUTTON) == LOW) {
    particle_type = (particle_type+1) % 6;
    loop_counter = 0;
  }  
  if (loop_counter != 0) return;
  delay(100);
  
  PM25_AQI_Data data;
  current_state =IDLE;
  signalState(current_state); 
  delay(100);
  
  if (! aqi.read(&data)) {
    Serial.println("Could not read from AQI");
    current_state = BADREADING;
    signalState(current_state);       
    delay(500);  // try again in a bit!
    return;
  
  }
  //Serial.println("AQI reading success");
  current_state = GOODREADING;
  signalState(current_state);  

  //Serial.println();
  //Serial.println(F("---------------------------------------"));
  //Serial.println(F("Concentration Units (standard)"));
  //Serial.println(F("---------------------------------------"));
  //Serial.print(F("PM 1.0: ")); Serial.print(data.pm10_standard);
  //Serial.print(F("\t\tPM 2.5: ")); Serial.print(data.pm25_standard);
  //Serial.print(F("\t\tPM 10: ")); Serial.println(data.pm100_standard);
  //Serial.println(F("Concentration Units (environmental)"));
  //Serial.println(F("---------------------------------------"));
  //Serial.print(F("PM 1.0: ")); Serial.print(data.pm10_env);
  //Serial.print(F("\t\tPM 2.5: ")); Serial.print(data.pm25_env);
  //Serial.print(F("\t\tPM 10: ")); Serial.println(data.pm100_env);
  //Serial.println(F("---------------------------------------"));
  //Serial.print(F("Particles > 0.3um / 0.1L air:")); Serial.println(data.particles_03um);
  //Serial.print(F("Particles > 0.5um / 0.1L air:")); Serial.println(data.particles_05um);
  //Serial.print(F("Particles > 1.0um / 0.1L air:")); Serial.println(data.particles_10um);
  //Serial.print(F("Particles > 2.5um / 0.1L air:")); Serial.println(data.particles_25um);
  //Serial.print(F("Particles > 5.0um / 0.1L air:")); Serial.println(data.particles_50um);
  //Serial.print(F("Particles > 10 um / 0.1L air:")); Serial.println(data.particles_100um);
  //Serial.println(F("---------------------------------------"));

  switch(particle_type) {
    case 0: display_particle_count(0.3, data.particles_03um); break;
    case 1: display_particle_count(0.5, data.particles_05um); break;
    case 2: display_particle_count(1.0, data.particles_10um); break;
    case 3: display_particle_count(2.5, data.particles_25um); break;
    case 4: display_particle_count(5.0, data.particles_50um); break;
    case 5: display_particle_count(10.0, data.particles_10um); break;
  }
  

}
