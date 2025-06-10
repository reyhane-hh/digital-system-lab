#include <Arduino.h>
#include <BasicEncoder.h>
#define CLK 2
#define DT 3
#define SW A4
#define MOTOR 9
#define POT A2
const int hallPin = 5;
int value = 0;
volatile int count = 0;
unsigned long lastTime = 0;
unsigned long interval = 1000;
double rpm = 0;
bool currentState = 0;
int counter = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir ="";
unsigned long lastButtonPress = 0;

const int latchPin = 6;
const int clockPin = 7;
const int dataPin = 8;

const byte digits[16] = {
  B11000000, 
  B11111001, 
  B10100100, 
  B10110000, 
  B10011001, 
  B10010010, 
  B10000010, 
  B11111000, 
  B10000000, 
  B10010000, 
  B10001000, 
  B10000011, 
  B11000110, 
  B10100001, 
  B10000110, 
  B10001110  
};

const byte digitPositions[4] = {
  B00001000, 
  B00000100,
  B00000010,
  B00000001  
};


int d0 = 0, d1 = 0, d2 = 0, d3 = 0;

void setup() {
  pinMode(MOTOR, OUTPUT);
  pinMode(hallPin, INPUT_PULLUP);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(CLK,INPUT);
  pinMode(DT,INPUT);
  pinMode(SW, INPUT_PULLUP);

  Serial.begin(9600);

  lastStateCLK = digitalRead(CLK);
  Serial.begin(9600);
  Serial.println(F("Polling in loop()"));
}
void sevenSeg(int counter){
  int d0 = (counter / 1) % 10;
  int d1 = (counter / 10) % 10;
  int d2 = (counter / 100) % 10;
  int d3 = (counter / 1000) % 10;

  for (int i = 0; i < 4; i++) {
    digitalWrite(latchPin, LOW);
    int val;
    if (i == 0) val = d0;
    else if (i == 1) val = d1;
    else if (i == 2) val = d2;
    else val = d3;

    if (val != -1)
      shiftOut(dataPin, clockPin, MSBFIRST, digits[val]);
    else
      shiftOut(dataPin, clockPin, MSBFIRST, 0xFF); 

    shiftOut(dataPin, clockPin, MSBFIRST, digitPositions[i]);
    digitalWrite(latchPin, HIGH);
  }
}



void loop() {

  currentStateCLK = digitalRead(CLK);

  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){

      if (digitalRead(DT) != currentStateCLK) {
          counter --;
           currentDir ="CCW";
      } else {
           counter ++;
           currentDir ="CW";
       }
 
     
    }

    lastStateCLK = currentStateCLK;

    int btnState = digitalRead(SW);

    if (btnState == LOW) {
        if (millis() - lastButtonPress > 50) {
            Serial.println("Button pressed!");
            counter = 0;
            value = 0;
        }
 
        lastButtonPress = millis();
    }
  constrain(counter,0,255);
  Serial.println(counter);
  analogWrite(MOTOR, value);
    currentState = digitalRead(hallPin);
  if (currentState == LOW) {
    count++;
    Serial.print("Round: ");
    Serial.println(count);
    while(digitalRead(hallPin)==0);
  }


  if ((millis() - lastTime) >= interval) {
    rpm = (count * 60.0/2); 
    Serial.print("RPM: ");
    Serial.println(rpm);

    count = 0;
    lastTime = millis();
  }
   delay(1);
  constrain(counter,0,9999);
  sevenSeg(rpm);
}