//www.elegoo.com
//2016.12.12

int latch=28;  //74HC595  pin 9 STCP
int clock=26; //74HC595  pin 10 SHCP
int data=24;   //74HC595  pin 8 DS

int pinDigits[4]={49, 51, 53, 47};

unsigned char table[]=
{0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c
,0x39,0x5e,0x79,0x71,0x00};

void setup() {
  pinMode(latch,OUTPUT);
  pinMode(clock,OUTPUT);
  pinMode(data,OUTPUT);
  for (short digit=0; digit<4; digit++) {
    pinMode(pinDigits[digit], OUTPUT);
    digitalWrite(pinDigits[digit], HIGH);
  }

}
void Display(unsigned char num, short digit)
{
  for (short _digit=0; _digit<4; _digit++) digitalWrite(pinDigits[_digit], digit==_digit ? LOW : HIGH);
  digitalWrite(latch,LOW);
  shiftOut(data,clock,MSBFIRST,table[num]);
  digitalWrite(latch,HIGH);
  
  
}
void loop() {
  Display(10, 0);
  delay(6);
  Display(11, 1);
  delay(6);
  Display(12, 2);
  delay(6);
  Display(13, 3);
  delay(6);
  
}
