//www.elegoo.com
//2016.12.12

// define the LED digit patterns, from 0 - 9
// 1 = LED on, 0 = LED off, in this order:
//                74HC595 pin     Q0,Q1,Q2,Q3,Q4,Q5,Q6,Q7 
//                Mapping to      a,b,c,d,e,f,g of Seven-Segment LED
byte seven_seg_digits[10] = { B11111101,  // = 0
                              B01100001,  // = 1
                              B11011011,  // = 2
                              B11110011,  // = 3
                              B01100111,  // = 4
                              B10110111,  // = 5
                              B10111111,  // = 6
                              B11100001,  // = 7
                              B11111111,  // = 8
                              B11100111   // = 9
                             };
 
// connect to the ST_CP of 74HC595 (pin 3,latch pin)
int latchPin = 26;
// connect to the SH_CP of 74HC595 (pin 4, clock pin)
int clockPin = 28;
// connect to the DS of 74HC595 (pin 2)
int dataPin = 24;
 
void setup() {
  // Set latchPin, clockPin, dataPin as output
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}
 
// display a number on the digital segment display
void sevenSegWrite(byte digit) {
  // set the latchPin to low potential, before sending data
  digitalWrite(latchPin, LOW);
     
  // the original data (bit pattern)
  shiftOut(dataPin, clockPin, LSBFIRST, seven_seg_digits[digit]);  
 
  // set the latchPin to high potential, after sending data
  digitalWrite(latchPin, HIGH);
}
 
void loop() {       
  // count from 9 to 0
  for (byte digit = 10; digit > 0; --digit) {
    delay(1000);
    sevenSegWrite(digit - 1); 
  }
   
  // suspend 4 seconds
  delay(3000);
}
