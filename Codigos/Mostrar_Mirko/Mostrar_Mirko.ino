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

#define BTN1 32
#define BTN2 33
#define Puerta_Sensor 26
#define LDR 35
#define Rele 14
#define Led1 16
#define Led2 17


/// Variables
uint8_t Maquina;



///Umbrales
int Umbral_LDR = 100;
int Variable_Rele = 0;
int umb_Tem = 20;



enum Estados { Espera_Mostrar_Temp,
               Mostrar_Temp,
               Espera_LDR,
               Mostrar_LDR,
               Subir_LDR,
               Bajar_LDR,
               Espera_RELE,
               Pantalla_Rele,
               Prender_Rele,
               Apagar_Rele,
               Espera_Tem,
               Modificar_Tem,
               Subir_Tem,
               Bajar_Tem,
               Espera_Mostrar_Puerta,
               Mostrar_Puerta



};

void setup() {
  pinMode(BTN1, INPUT);
  pinMode(BTN2, INPUT);
  pinMode(Puerta_Sensor, INPUT);
  pinMode(LDR, INPUT);
  pinMode(Rele, OUTPUT);
  pinMode(Led1, OUTPUT);
  pinMode(Led2, OUTPUT);
  // Inicializa la comunicación I2C
  Wire.begin();

  // Inicializa el LCD
  lcd.init();

  // Enciende la retroiluminación del LCD
  lcd.backlight();  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:

  float AHT_Tem = aht10.readTemperature();
  if (AHT_Tem >= umb_Tem) {
    digitalWrite(Led1, HIGH);
  } else {
    digitalWrite(Led1, LOW);
  }
  if (analogRead(LDR) >= Umbral_LDR) {
    digitalWrite(Led2, HIGH);
  } else {
    digitalWrite(Led2, LOW);
  }

  switch (Maquina) {
    case Mostrar_Temp:
      lcd.setCursor(0, 0);
      lcd.print("Temperatura es:");
      lcd.setCursor(0, 1);
      lcd.print(AHT_Tem);
      if (digitalRead(BTN1) == LOW && digitalRead(BTN2) == LOW) {
        Maquina = Espera_LDR;
      }
      break;
    case Espera_LDR:
      if (digitalRead(BTN1) == HIGH && digitalRead(BTN2) == HIGH) {
        Maquina = Mostrar_LDR;
        lcd.clear();
      }
      break;
    case Mostrar_LDR:
      lcd.setCursor(0, 0);
      lcd.print("La LUZ:");
      lcd.print(analogRead(LDR));
      lcd.setCursor(0, 1);
      lcd.print("Mi Umbral es:");
      lcd.print(Umbral_LDR);
      if (digitalRead(BTN1) == LOW) {
        Maquina = Subir_LDR;
      }
      if (digitalRead(BTN2) == LOW) {
        Maquina = Bajar_LDR;
      }
      if (digitalRead(BTN1) == LOW && digitalRead(BTN2) == LOW) {
        Maquina = Espera_RELE;
      }
      break;
    case Bajar_LDR:
      if (digitalRead(BTN2) == HIGH) {
        Umbral_LDR = Umbral_LDR - 10;
        Maquina = Mostrar_LDR;
        lcd.clear();
      }
      if (digitalRead(BTN1) == LOW && digitalRead(BTN2) == LOW) {
        Maquina = Espera_RELE;
      }
      break;
    case Subir_LDR:
      if (digitalRead(BTN1) == HIGH) {
        Umbral_LDR = Umbral_LDR + 10;
        Maquina = Mostrar_LDR;
        lcd.clear();
      }
      if (digitalRead(BTN1) == LOW && digitalRead(BTN2) == LOW) {
        Maquina = Espera_RELE;
      }
      break;
    case Espera_RELE:
      if (digitalRead(BTN1) == HIGH && digitalRead(BTN2) == HIGH) {
        Maquina = Pantalla_Rele;
        lcd.clear();
      }
      break;

    case Pantalla_Rele:
      lcd.setCursor(0, 0);
      lcd.print("El rele esta");
      if (Variable_Rele == 0) {
        lcd.setCursor(0, 1);
        lcd.print("APAGADO");
      }
      if (Variable_Rele == 1) {
        lcd.setCursor(0, 1);
        lcd.print("PRENDIDO");
      }
      if (digitalRead(BTN1) == LOW) {
        Maquina = Apagar_Rele;
      }
      if (digitalRead(BTN2) == LOW) {
        Maquina = Prender_Rele;
      }
      if (digitalRead(BTN1) == LOW && digitalRead(BTN2) == LOW) {
        Maquina = Espera_Tem;
      }
      break;
    case Apagar_Rele:
      if (digitalRead(BTN1) == HIGH) {
        Maquina = Pantalla_Rele;
        Variable_Rele = 0;
        lcd.clear();
        digitalWrite(Rele, LOW);
      }
      if (digitalRead(BTN1) == LOW && digitalRead(BTN2) == LOW) {
        Maquina = Espera_Tem;
      }
      break;
    case Prender_Rele:
      if (digitalRead(BTN2) == HIGH) {
        Maquina = Pantalla_Rele;
        Variable_Rele = 1;
        lcd.clear();
        digitalWrite(Rele, HIGH);
      }
      if (digitalRead(BTN1) == LOW && digitalRead(BTN2) == LOW) {
        Maquina = Espera_Tem;
      }
      break;

    case Espera_Tem:
      if (digitalRead(BTN1) == HIGH && digitalRead(BTN2) == HIGH) {
        lcd.clear();
        Maquina = Modificar_Tem;
      }
      break;

    case Modificar_Tem:
      lcd.setCursor(0, 0);
      lcd.print("Umbral Temperatura:");
      lcd.setCursor(0, 1);
      lcd.print(umb_Tem);
      if (digitalRead(BTN1) == LOW) {
        Maquina = Bajar_Tem;
      }
      if (digitalRead(BTN2) == LOW) {
        Maquina = Subir_Tem;
      }
      if (digitalRead(BTN1) == LOW && digitalRead(BTN2) == LOW) {
        Maquina = Espera_Mostrar_Puerta;
      }
      break;
    case Bajar_Tem:
      if (digitalRead(BTN1) == HIGH) {
        Maquina = Modificar_Tem;
        umb_Tem--;
        lcd.clear();
      }
      if (digitalRead(BTN1) == LOW && digitalRead(BTN2) == LOW) {
        Maquina = Espera_Mostrar_Puerta;
      }
      break;
    case Subir_Tem:
      if (digitalRead(BTN2) == HIGH) {
        Maquina = Modificar_Tem;
        umb_Tem++;
        lcd.clear();
      }
      if (digitalRead(BTN1) == LOW && digitalRead(BTN2) == LOW) {
        Maquina = Espera_Mostrar_Puerta;
      }
      break;
    case Espera_Mostrar_Puerta:
      if (digitalRead(BTN1) == HIGH && digitalRead(BTN2) == HIGH) {
        Maquina = Mostrar_Puerta;
        lcd.clear();
      }
      break;
    case Mostrar_Puerta:
      lcd.setCursor(0, 0);
      lcd.print("La puerta esta");
      if (digitalRead(Puerta_Sensor) == LOW) {
        lcd.setCursor(0, 1);
        lcd.print("Cerrada");
      } else {
        lcd.setCursor(0, 1);
        lcd.print("Abierta");
      }
      if (digitalRead(BTN1) == LOW && digitalRead(BTN2) == LOW) {
        Maquina = Espera_Mostrar_Temp;
      }
      break;
    case Espera_Mostrar_Temp:
      if (digitalRead(BTN1) == HIGH && digitalRead(BTN2) == HIGH) {
        lcd.clear();
        Maquina = Mostrar_Temp;
      }
      break;
  }
}
