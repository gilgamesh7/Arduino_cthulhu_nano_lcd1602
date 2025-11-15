#include <LiquidCrystal_I2C.h>

// Initialize the library with the numbers of the interface pins
LiquidCrystal_I2C lcd(0x27, 16, 2); // the LCD is 16 units wide, 2 units down

String myString = "ph'nglui mglw'nafh cthulhu r'lyeh wgah'nagl fhtagn !   ";

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.clear();

  // Print only the first 16 chars onto row 0
  lcd.setCursor(0, 0);
  lcd.print(myString.substring(0, 16)); 
}

void loop() {

  for (int i = 0; i < myString.length(); i++) {

    lcd.clear();

    // Print the next 16-character window
    lcd.setCursor(0, 0);
    lcd.print(myString.substring(i, i + 16));

    delay(500); // adjust speed here
  }
}










