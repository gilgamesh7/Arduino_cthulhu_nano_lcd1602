#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

SoftwareSerial mp3Serial(10, 11); // RX, TX
DFRobotDFPlayerMini mp3;

const int buttonPin = 2;
bool lastButtonState = HIGH;

String myString = "ph'nglui mglw'nafh cthulhu r'lyeh wgah'nagl fhtagn !   ";

void setup() {

  // LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();

  // Button
  pinMode(buttonPin, INPUT_PULLUP);

  // Serial
  mp3Serial.begin(9600);

  if (!mp3.begin(mp3Serial)) {
    lcd.setCursor(0, 1);
    lcd.print("MP3 ERROR");
    while(true);
  }

  mp3.volume(25);  // 0–30
}

void loop() {

  // ---- SCROLL TEXT ----
  for (int i = 0; i < myString.length(); i++) {

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(myString.substring(i, i + 16));

    // ---- CHECK BUTTON ----
    bool currentButtonState = digitalRead(buttonPin);

    if (lastButtonState == HIGH && currentButtonState == LOW) {
      mp3.play(1);  // Plays 0001.mp3
      delay(300);   // debounce
    }

    lastButtonState = currentButtonState;

    delay(500);
  }
}
