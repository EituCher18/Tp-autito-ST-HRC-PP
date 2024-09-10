#include <Wire.h> // Incluye la librería Wire para la comunicación I2C
#include <LiquidCrystal_I2C.h> // Incluye la librería LiquidCrystal_I2C
#include <Arduino.h>
// Dirección I2C de la pantalla LCD
int lcdAddress = 0x27; // Puede variar según tu módulo I2C

// Crea una instancia de LiquidCrystal_I2C
LiquidCrystal_I2C lcd(lcdAddress, 16, 2);

void setup() {
  // Inicializa la comunicación I2C
  Wire.begin();

  // Inicializa el LCD
  lcd.init();

  // Enciende la retroiluminación del LCD
  lcd.backlight();
}

void loop() {
 lcd.setCursor(4,0);
 lcd.print("Hola Mirko");
 lcd.setCursor(0,1);
 lcd.print("Prieto Me aproba");
 }
