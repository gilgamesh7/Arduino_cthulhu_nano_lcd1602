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

// MP3
SoftwareSerial mp3Serial(10, 11);
DFRobotDFPlayerMini mp3;

// Button
const int buttonPin = 2;
bool lastButtonState = HIGH;

// Ultrasonic
const int trigPin = 7;
const int echoPin = 6;

long duration;
int distance;

const int triggerDistance = 20;
unsigned long lastTriggerTime = 0;
const unsigned long cooldown = 3000;

// Timing
unsigned long explosionStart = 0;
bool explosionActive = false;

// 🔧 FUNCTION PROTOTYPES (fixes your error)
int getDistance();
void runExplosionAnimation();
void runCthulhuAwakensEffect();

void setup() {

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true);
  }

  display.clearDisplay();

  pinMode(buttonPin, INPUT_PULLUP);

  mp3Serial.begin(9600);

  if (!mp3.begin(mp3Serial)) {
    while (true);
  }

  mp3.volume(26);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {

  // BUTTON
  bool currentButtonState = digitalRead(buttonPin);
  if (lastButtonState == HIGH && currentButtonState == LOW) {
    mp3.play(1);
    delay(300);
  }
  lastButtonState = currentButtonState;

  // ULTRASONIC
  int dist = getDistance();

  if (dist > 0 && dist < triggerDistance) {
    if (millis() - lastTriggerTime > cooldown) {

      mp3.play(2);
      explosionStart = millis();
      explosionActive = true;

      lastTriggerTime = millis();
    }
  }

  // ANIMATION CONTROL
  if (explosionActive) {

    runCthulhuAwakensEffect();

    if (millis() - explosionStart > 15000) {
      explosionActive = false;
      display.clearDisplay();
      display.display();
    }

  } else {
    runExplosionAnimation(); // idle pulsing
  }
}

// ----------------------------

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

// ----------------------------

void runExplosionAnimation() {

  static int radius = 2;
  static int direction = 1;

  display.clearDisplay();

  int centerX = SCREEN_WIDTH / 2;
  int centerY = SCREEN_HEIGHT / 2;

  display.drawCircle(centerX, centerY, radius, SSD1306_WHITE);
  display.drawCircle(centerX, centerY, radius / 2, SSD1306_WHITE);

  for (int i = 0; i < 8; i++) {
    int angle = random(0, 360);
    float rad = angle * 3.14 / 180;

    int x2 = centerX + cos(rad) * (radius + random(5, 15));
    int y2 = centerY + sin(rad) * (radius + random(5, 15));

    display.drawLine(centerX, centerY, x2, y2, SSD1306_WHITE);
  }

  display.display();

  radius += direction;

  if (radius > 15) direction = -1;
  if (radius < 3) direction = 1;

  delay(40);
}

// CTHULHU AWAKENING EFFECT
void runCthulhuAwakensEffect() {

  const int numChars = 12;

  static int x[numChars];
  static int y[numChars];
  static int dx[numChars];
  static int dy[numChars];
  static char symbols[numChars];

  static bool initialized = false;

  if (!initialized) {
    for (int i = 0; i < numChars; i++) {
      x[i] = random(0, SCREEN_WIDTH);
      y[i] = random(0, SCREEN_HEIGHT);

      dx[i] = random(-1, 2);
      dy[i] = random(-1, 2);

      char charset[] = "!@#$%^&*(){}<>?/|\\~";
      symbols[i] = charset[random(0, sizeof(charset) - 1)];
    }
    initialized = true;
  }

  // 🔥 WHITE BACKGROUND
  display.fillScreen(SSD1306_WHITE);

  for (int i = 0; i < numChars; i++) {

    display.setCursor(x[i], y[i]);
    display.setTextSize(1);

    // ⚫ BLACK TEXT
    display.setTextColor(SSD1306_BLACK);

    display.write(symbols[i]);

    x[i] += dx[i];
    y[i] += dy[i];

    if (random(0, 10) > 7) {
      dx[i] = random(-1, 2);
      dy[i] = random(-1, 2);
    }

    if (x[i] < 0) x[i] = SCREEN_WIDTH;
    if (x[i] > SCREEN_WIDTH) x[i] = 0;
    if (y[i] < 0) y[i] = SCREEN_HEIGHT;
    if (y[i] > SCREEN_HEIGHT) y[i] = 0;

    if (random(0, 20) == 0) {
      // char charset[] = "!@#$%^&*(){}<>?/|\\~CTHULHU";
      char charset[] = "!@#$%CTHUL";
      symbols[i] = charset[random(0, sizeof(charset) - 1)];
    }

    if (random(0, 70) == 0) {
      display.fillScreen(SSD1306_BLACK);
      display.display();
      delay(20);
    }
  }

  display.display();

  delay(50);
}
