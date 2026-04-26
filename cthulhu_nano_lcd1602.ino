#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

// OLED config
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// MP3
SoftwareSerial mp3Serial(10, 11);
DFRobotDFPlayerMini mp3;

// Pins
const int buttonPin = 2;
const int trigPin = 7;
const int echoPin = 6;

// State
bool lastButtonState = HIGH;
bool explosionActive = false;
bool awakeningMessageActive = false;

// Timing
unsigned long lastTriggerTime = 0;
unsigned long explosionStart = 0;
const unsigned long cooldown = 3000;

// Ultrasonic
long duration;
int distance;
const int triggerDistance = 20;

// LCD state
int awakeningOffset = 0;
int hymnOffset = 0;
unsigned long lastLCDUpdate = 0;

// OLED timing
unsigned long lastOLEDUpdate = 0;

// Text
String cthulhuHymn = "ph'nglui mglw'nafh cthulhu r'lyeh wgah'nagl fhtagn !   ";
String awakeningMsg = "IA! IA! CTHULHU AWAKENS! PRAISE CTHULHU !  ";

// ----------------------------

void setup() {

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  lcd.init();
  lcd.backlight();
  lcd.clear();

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  mp3Serial.begin(9600);
  mp3.begin(mp3Serial);
  mp3.volume(26);
}

// ----------------------------

void loop() {

  handleButton();
  handleUltrasonic();

  updateOLED();
  updateLCD();
}

// ----------------------------
// INPUT HANDLING
// ----------------------------

void handleButton() {
  bool current = digitalRead(buttonPin);

  if (lastButtonState == HIGH && current == LOW) {
    mp3.play(1);
  }

  lastButtonState = current;
}

void handleUltrasonic() {
  int dist = getDistance();

  if (dist > 0 && dist < triggerDistance) {
    if (millis() - lastTriggerTime > cooldown) {

      mp3.play(2);

      awakeningMessageActive = true;
      awakeningOffset = 0;

      explosionActive = true;
      explosionStart = millis();

      lastTriggerTime = millis();
    }
  }
}

// ----------------------------
// OLED (NON-BLOCKING)
// ----------------------------

void updateOLED() {

  if (millis() - lastOLEDUpdate < 50) return;
  lastOLEDUpdate = millis();

  if (explosionActive) {

    runCthulhuAwakensEffect();

    if (millis() - explosionStart > 15000) {
      explosionActive = false;
      display.clearDisplay();
      display.display();
    }

  } else {
    runExplosionAnimation();
  }
}

// ----------------------------
// LCD (NON-BLOCKING)
// ----------------------------

void updateLCD() {

  if (millis() - lastLCDUpdate < 220) return;
  lastLCDUpdate = millis();

  lcd.clear();
  lcd.setCursor(0, 0);

  if (awakeningMessageActive) {

    // Flash effect (very short, acceptable)
    lcd.noBacklight();
    delay(10);
    lcd.backlight();

    lcd.print(getWindow(awakeningMsg, awakeningOffset));

    awakeningOffset++;

    if (awakeningOffset > awakeningMsg.length() + 16) {
      awakeningMessageActive = false;
      awakeningOffset = 0;
    }

  } else {
    lcd.print(getWindow(cthulhuHymn, hymnOffset));
    hymnOffset = (hymnOffset + 1) % cthulhuHymn.length();
  }
}

// ----------------------------
// STRING WINDOW HELPER
// ----------------------------

String getWindow(String text, int offset) {

  String out = "";
  int len = text.length();

  for (int i = 0; i < 16; i++) {
    out += text[(offset + i) % len];
  }

  return out;
}

// ----------------------------
// ULTRASONIC
// ----------------------------

int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2;
}

// ----------------------------
// OLED EFFECTS (UNCHANGED CORE)
// ----------------------------

void runExplosionAnimation() {

  static int radius = 2;
  static int direction = 1;

  display.clearDisplay();

  int cx = SCREEN_WIDTH / 2;
  int cy = SCREEN_HEIGHT / 2;

  display.drawCircle(cx, cy, radius, SSD1306_WHITE);
  display.drawCircle(cx, cy, radius / 2, SSD1306_WHITE);

  for (int i = 0; i < 8; i++) {
    float rad = random(0, 360) * 3.14 / 180;

    int x2 = cx + cos(rad) * (radius + random(5, 15));
    int y2 = cy + sin(rad) * (radius + random(5, 15));

    display.drawLine(cx, cy, x2, y2, SSD1306_WHITE);
  }

  display.display();

  radius += direction;
  if (radius > 15) direction = -1;
  if (radius < 3) direction = 1;
}

// ----------------------------

void runCthulhuAwakensEffect() {

  const int numChars = 12;

  static int x[numChars], y[numChars], dx[numChars], dy[numChars];
  static char symbols[numChars];
  static bool init = false;

  if (!init) {
    for (int i = 0; i < numChars; i++) {
      x[i] = random(0, SCREEN_WIDTH);
      y[i] = random(0, SCREEN_HEIGHT);
      dx[i] = random(-1, 2);
      dy[i] = random(-1, 2);
      symbols[i] = "!@#$%^&*(){}<>?"[random(0, 16)];
    }
    init = true;
  }

  display.fillScreen(SSD1306_WHITE);

  for (int i = 0; i < numChars; i++) {

    display.setCursor(x[i], y[i]);
    display.setTextColor(SSD1306_BLACK);
    display.setTextSize(1);
    display.write(symbols[i]);

    x[i] += dx[i];
    y[i] += dy[i];

    if (x[i] < 0 || x[i] > SCREEN_WIDTH) dx[i] *= -1;
    if (y[i] < 0 || y[i] > SCREEN_HEIGHT) dy[i] *= -1;
  }

  display.display();
}
