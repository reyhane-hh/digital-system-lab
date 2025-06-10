#include <Keypad.h>

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

const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'F','0','E','D'}
};

byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {A2, A3, A4, A5};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

String hexMap = "0123456789ABCDEF";

int d0 = 0, d1 = 0, d2 = 0, d3 = 0;

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  Serial.begin(9600);
}


void loop() {
  char key = customKeypad.getKey();

  if (key != NO_KEY) {
    int index= key;
    index = index -48;
 if (key >= 'A' && key <= 'F') {
    index = key - 'A' + 10; 
}

    Serial.println(index);

    if (index != -1) {
      d3 = d2;
      d2 = d1;
      d1 = d0;
      d0 = index;
    }
  }
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
    delay(5);
  }
}
