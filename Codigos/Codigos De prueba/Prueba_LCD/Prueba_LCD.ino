#include <Wire.h>               // Incluye la librería Wire para la comunicación I2C
#include <LiquidCrystal_I2C.h>  // Incluye la librería LiquidCrystal_I2C
#include <Arduino.h>

#include <AHTxx.h>

float AHT_Tem;
float AHT_Hum;
AHTxx aht10(AHTXX_ADDRESS_X38, AHT1x_SENSOR);

// Dirección I2C de la pantalla LCD
int lcdAddress = 0x27;  // Puede variar según tu módulo I2C

// Crea una instancia de LiquidCrystal_I2C
LiquidCrystal_I2C lcd(lcdAddress, 16, 2);

long Ms;

void Codigo_Pruba();

void Codigo_Pruba_Con_AHT();

void setup() {
  // Inicializa la comunicación I2C
  Wire.begin();

  // Inicializa el LCD
  lcd.init();

  // Enciende la retroiluminación del LCD
  lcd.backlight();
  Ms = 0;
}

void loop() {
  Codigo_Pruba();
}

void Codigo_Pruba() {
  lcd.setCursor(4, 0);
  lcd.print("Hola Mirko");
  lcd.setCursor(0, 1);
  lcd.print("Prieto Me aproba");
}

void Codigo_Pruba_Con_AHT() {

  AHT_Tem = aht10.readTemperature();
  lcd.setCursor(0, 0);
  lcd.print("La Temp es");
  lcd.setCursor(11, 0);
  lcd.print(AHT_Tem);
  lcd.setCursor(15, 0);
  lcd.print("C");
  AHT_Hum = aht10.readHumidity();
  lcd.setCursor(0, 1);
  lcd.print("La Hum es");
  lcd.setCursor(10, 1);
  lcd.print(AHT_Hum);
  lcd.setCursor(14, 1);
  lcd.print("%");

  if (millis() - Ms >= 1000) {
    Ms = millis();
    lcd.clear();
  }
}
