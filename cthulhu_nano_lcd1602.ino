#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

SoftwareSerial mp3Serial(10, 11); // RX, TX
DFRobotDFPlayerMini mp3;

const int buttonPin = 2;
bool lastButtonState = HIGH;

// Ultrasonic sensor pins
const int trigPin = 7;
const int echoPin = 6;

long duration;
int distance;

// Distance threshold (cm)
const int triggerDistance = 20;

// Cooldown to prevent repeat triggering
unsigned long lastTriggerTime = 0;
const unsigned long cooldown = 3000; // 3 seconds

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

  // Ultrasonic sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
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

    // ---- ULTRASONIC CHECK ----
    int dist = getDistance();

    if (dist > 0 && dist < triggerDistance) {
      if (millis() - lastTriggerTime > cooldown) {
        mp3.play(2);  // 0002.mp3

        showAwakeningMessage();  // 🔥 Dramatic display

        lastTriggerTime = millis();
      }
    }


    delay(400);
  }
}

int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  return distance;
}

void showAwakeningMessage() {

  String msg = "IA! IA! CTHULHU AWAKENS!   ";

  for (int i = 0; i < msg.length(); i++) {

    // Flash effect
    lcd.noBacklight();
    delay(80);
    lcd.backlight();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(msg.substring(i, i + 16));

    delay(220);
  }

  // Ensure backlight stays ON after ritual
  lcd.backlight();
}
