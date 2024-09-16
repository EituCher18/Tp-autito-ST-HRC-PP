  #include "Declaraciones.h"
void setupLibrerias() {


  EasyBuzzer.setPin(BUZZER_PIN);

  // Inicializa la comunicación I2C
  Wire.begin();

  // Inicializa el LCD
  lcd.init();

  // Enciende la retroiluminación del LCD
  lcd.backlight();

  Serial.println("AHT10");  // Se imprime el nombre de sensor
  if (!myAHT10.begin()) {   // Si la comunicación con el sensor falla se imprime el un mensaje de error
    Serial.println("Error no se el sensor!");
    while (1)
      ;
  }
}
