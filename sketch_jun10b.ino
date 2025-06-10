#include "DallasTemperature.h"
#include "OneWire.h"
#define ONE_WIRE_BUS A1
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

int latchPin = 6;
int clockPin = 7;
int dataPin = 8;

byte digits[11] = {
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
  B01111111
};

byte digitPositions[4] = {
  B00000001,
  B00000010,
  B00000100,
  B00001000
};

void setup() {
  Serial.begin(9600);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void displayDigit(int digit, int position) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, digits[digit]);
  shiftOut(dataPin, clockPin, MSBFIRST, digitPositions[position]);
  digitalWrite(latchPin, HIGH);
}

void displayTemperature(float temp) {
  int tempInt = temp * 10;
  
  int digit0 = (tempInt / 1) % 10;
  int digit1 = (tempInt / 10) % 10;
  int digit2 = (tempInt / 100) % 10;
  
  for(int i = 0; i < 50; i++) {
    if(digit2 > 0) {
      displayDigit(digit2, 0);
      delay(5);
    }
    displayDigit(digit1, 1);
    delay(5);
    displayDigit(10, 1);
    delay(5);
    displayDigit(digit0, 2);
    delay(5);
  }
}

void loop() {
  float tempC = sensors.getTempCByIndex(0); 
  
  Serial.print("Temp=");
  Serial.println(tempC, 1);
  
  displayTemperature(tempC);
  
  delay(100);
}