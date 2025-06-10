#include <LiquidCrystal.h>
#define MOTOR 9
#define POT A2
const int hallPin = 2;
int latchPin = 6;
int clockPin = 7;
int dataPin = 8;
int value = 0;
volatile int count = 0;
unsigned long lastTime = 0;
unsigned long interval = 1000; 
double rpm = 0;
bool currentState = 0;
const int rs = 6, en = 7, d4 = 10, d5 = 11, d6 = 12, d7 = 13;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
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
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(MOTOR, OUTPUT);
  pinMode(hallPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(hallPin), countPulse, FALLING);
  Serial.begin(9600);
}
void displayDigit(int digit, int position) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, digits[digit]);
  shiftOut(dataPin, clockPin, MSBFIRST, digitPositions[position]);
  digitalWrite(latchPin, HIGH);
}
void displayRPM(float pwm) {
  int pwmInt = pwm * 10;
  
  int digit0 = (pwmInt / 100) % 10;
  int digit1 = (pwmInt / 1000) % 10;
  int digit2 = (pwmInt / 10000) % 10;
  
  for(int i = 0; i < 50; i++) {
    if(digit2 > 0) {
      displayDigit(digit2, 0);
      delay(5);
    }
    displayDigit(digit1, 1);
    delay(5);
    displayDigit(digit0, 2);
    delay(5);
    displayDigit(0, 3);
    delay(5);
  }
}
void loop() {
  value = analogRead(POT);
  analogWrite(MOTOR, value / 4);
    currentState = digitalRead(hallPin);
  if (currentState == LOW) {
    count++;
    Serial.print("Round: ");
    Serial.println(count);
    while(digitalRead(hallPin)==0);
  }

  if ((millis() - lastTime) >= interval) {
    rpm = (count * 60/2);
    Serial.print("RPM: ");
    Serial.println(rpm);

    count = 0;
    lastTime = millis();
  }
  lcd.begin(16, 2);
  lcd.print("RPM=");
  lcd.print(rpm,0);
  lcd.print(" u/min");
  displayRPM(rpm);
}

void countPulse() {
  count++;
}
