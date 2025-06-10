#include <Keypad.h>
#include <LiquidCrystal.h>

const int rs = 6, en = 7, d4 = 10, d5 = 11, d6 = 12, d7 = 13;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

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

void setup() {
  lcd.begin(16, 2);
  lcd.clear();
  Serial.begin(9600);
}

void loop() {
  char key = customKeypad.getKey();

  if (key != NO_KEY) {
    key = toupper(key);
    int index = hexMap.indexOf(key);

    if (index != -1) {
      int col = index; 
      int row = (index % 2 == 0) ? 0 : 1;

      lcd.setCursor(col, row);
      lcd.print(key);
      Serial.print(key);
    }
  }
}
