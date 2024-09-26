#include "Declaraciones.h"

uint8_t Maquina;

// falta agregar el tema del buzzer
void setup() {
  // put your setup code here, to run once:
  setupLibrerias();
  SetupContadores();
  SetupEeprom();
  pines();
  Maquina = INIT_HUMEDAD;
  Maquina_Baliza = On;
}

void loop() {
  Eeprom();
  Baliza();
  Venti();
  switch (Maquina) {
    case INIT_HUMEDAD:
      if (digitalRead(Sensor_Puerta) == LOW) {
        lcd.clear();
        Maquina = Door;
        Puerta = INIT_HUMEDAD;
      }
      lcd.setCursor(0, 0);
      lcd.print("La humedad es:");
      lcd.setCursor(0, 1);
      lcd.print(myAHT10.readHumidity());
      lcd.setCursor(7, 1);
      lcd.print("g/m3");
      if (digitalRead(BTN_CHANGE) == LOW) {
        Maquina = Espera_Pantalla2;
      }

      if (millis() - Ms >= 2000) {
        Maquina = INIT_TEMPERATURA;
        Ms = millis();
        lcd.clear();
      }
      break;
    case INIT_TEMPERATURA:
      if (digitalRead(Sensor_Puerta) == LOW) {
        lcd.clear();
        Maquina = Door;
        Puerta = INIT_TEMPERATURA;
      }
      lcd.setCursor(0, 0);
      lcd.print("Temperatura es:");
      lcd.setCursor(0, 1);
      lcd.print(myAHT10.readTemperature());
      lcd.setCursor(7, 1);
      lcd.print("C");
      if (millis() - Ms >= 2000) {
        Maquina = INIT_HUMEDAD;
        Ms = millis();
        lcd.clear();
      }
      if (digitalRead(BTN_CHANGE) == LOW) {
        Maquina = Espera_Pantalla2;
      }
      break;

    case Espera_Pantalla2:
      if (digitalRead(Sensor_Puerta) == LOW) {
        lcd.clear();
        Maquina = Door;
        Puerta = Espera_Pantalla2;
      }
      if (digitalRead(BTN_CHANGE) == HIGH) {
        Maquina = Pantalla2;
        lcd.clear();
      }
      break;

    case Pantalla2:
      if (digitalRead(Sensor_Puerta) == LOW) {
        lcd.clear();
        Maquina = Door;
        Puerta = Pantalla2;
      }
      lcd.setCursor(0, 0);
      lcd.print("Umbral de luz es");
      lcd.setCursor(0, 1);
      lcd.print(Umb_Lum);
      if (digitalRead(BTN_SUM) == LOW) {
        Maquina = Luz_SUM;
        Umb_Lum = Umb_Lum + 100;
      }
      if (digitalRead(BTN_RES) == LOW) {
        Maquina = Luz_Baj;
        Umb_Lum = Umb_Lum - 100;
      }
      if (digitalRead(BTN_CHANGE) == LOW) {
        Maquina = Espera_Pantalla3;
      }
      break;
    case Luz_Baj:
      if (digitalRead(Sensor_Puerta) == LOW) {
        lcd.clear();
        Maquina = Door;
        Puerta = Luz_Baj;
      }
      if (digitalRead(BTN_RES) == HIGH) {
        Maquina = Pantalla2;
        lcd.clear();
      }
      break;
    case Luz_SUM:
      if (digitalRead(Sensor_Puerta) == LOW) {
        lcd.clear();
        Maquina = Door;
        Puerta = Luz_SUM;
      }
      if (digitalRead(BTN_SUM) == HIGH) {
        Maquina = Pantalla2;
        lcd.clear();
      }
      break;
    case Espera_Pantalla3:
      if (digitalRead(Sensor_Puerta) == LOW) {
        lcd.clear();
        Maquina = Door;
        Puerta = Espera_Pantalla3;
      }
      if (digitalRead(BTN_CHANGE) == HIGH) {
        Maquina = Pantalla3;
        lcd.clear();
      }
      break;

    case Pantalla3:
      if (digitalRead(Sensor_Puerta) == LOW) {
        lcd.clear();
        Maquina = Door;
        Puerta = Pantalla3;
      }
      if (Ventilador = 1) {
        lcd.setCursor(0, 1);
        lcd.print("On");
      }
      if (Ventilador = 0) {

        lcd.setCursor(0, 1);
        lcd.print("Off");
      }

      lcd.setCursor(0, 0);
      lcd.print("Ventilador esta:");


      if (digitalRead(BTN_SUM) == LOW) {
        Ventilador = 1;
      }
      if (digitalRead(BTN_RES) == LOW) {
        Maquina = Ventilador_Down;
        Ventilador = 0;
      }
      if (digitalRead(BTN_CHANGE) == LOW) {
        Maquina = Espera_T_Eeprom;
      }
      break;
    case Ventilador_Down:
      if (digitalRead(Sensor_Puerta) == LOW) {
        lcd.clear();
        Maquina = Door;
        Puerta = Ventilador_Down;
      }
      if (digitalRead(BTN_RES) == HIGH) {
        lcd.clear();
        Maquina = Pantalla3;
      }
      break;
    case Ventilador_Up:
      if (digitalRead(Sensor_Puerta) == LOW) {
        lcd.clear();
        Maquina = Door;
        Puerta = Ventilador_Up;
      }
      if (digitalRead(BTN_SUM) == HIGH) {
        Maquina = Pantalla3;
      }
      break;
    case Espera_Pantalla4:
      if (digitalRead(Sensor_Puerta) == LOW) {
        lcd.clear();
        Maquina = Door;
        Puerta = Espera_Pantalla4;
      }
      if (digitalRead(BTN_CHANGE) == HIGH) {
        Maquina = Pantalla4;
        lcd.clear();
      }
      break;


    case Espera_T_Eeprom:
      if (digitalRead(Sensor_Puerta) == LOW) {
        lcd.clear();
        Maquina = Door;
        Puerta = Espera_T_Eeprom;
      }
      if (digitalRead(BTN_CHANGE) == HIGH) {
        lcd.clear();
        Maquina = T_Eeprom;
      }
      break;
    case T_Eeprom:

      lcd.setCursor(0, 0);
      lcd.print("El intervalo de");
      lcd.setCursor(0, 1);
      lcd.print("guardado");
      lcd.setCursor(9, 1);
      lcd.print(Intervalo_Corto);
      lcd.setCursor(13, 1);
      lcd.print("Seg");
      if (digitalRead(Sensor_Puerta) == LOW) {
        lcd.clear();
        Maquina = Door;
        Puerta = T_Eeprom;
      }
      if (digitalRead(BTN_CHANGE) == LOW) {
        Maquina = Espera_Pantalla4;
      }

      if (digitalRead(BTN_SUM == LOW)) {
        Intervalo++;
        lcd.clear();
        Maquina = Sub_Eeprom;
      }

      if (digitalRead(BTN_RES) == LOW && Intervalo >= 30000) {
        Intervalo--;
        lcd.clear();
        Maquina = Baj_Eeprom;
      }
      break;
    case Sub_Eeprom:
      if (digitalRead(Sensor_Puerta) == LOW) {
        lcd.clear();
        Maquina = Door;
        Puerta = Sub_Eeprom;
      }

      if (digitalRead(BTN_SUM) == HIGH) {
        Maquina = T_Eeprom;
      }
      break;

    case Baj_Eeprom:

      if (digitalRead(Sensor_Puerta) == LOW) {
        lcd.clear();
        Maquina = Door;
        Puerta = Baj_Eeprom;
      }
      if (digitalRead(BTN_RES) == HIGH) {
        Maquina = T_Eeprom;
      }
      break;

    case Pantalla4:
      if (digitalRead(Sensor_Puerta) == LOW) {
        lcd.clear();
        Maquina = Door;
        Puerta = Pantalla4;
      }
      lcd.setCursor(0, 0);
      lcd.print("Balizas");
      if (digitalRead(BTN_CHANGE == LOW)) {
        Maquina = Espera_Pantalla_INIT;
      }
      if (Leds = 1) {
        lcd.setCursor(0, 1);
        lcd.print("ON");
      }
      if (Leds = 0) {
        lcd.setCursor(0, 1);
        lcd.print("OFF");
      }
      if (digitalRead(BTN_SUM) == LOW) {
        Leds = 1;
        Maquina = Sum_Leds;
      }
      if (digitalRead(BTN_RES) == LOW)

      {
        Leds = 0;
        Maquina = Rest_Leds;
      }
      break;
    case Sum_Leds:
      if (digitalRead(Sensor_Puerta) == LOW) {
        lcd.clear();
        Maquina = Door;
        Puerta = Sum_Leds;
      }
      if (digitalRead(BTN_CHANGE) == HIGH) {
        Maquina = Pantalla4;
        lcd.clear();
      }
      break;
    case Rest_Leds:
      if (digitalRead(Sensor_Puerta) == LOW) {
        lcd.clear();
        Maquina = Door;
        Puerta = Rest_Leds;
      }
      if (digitalRead(BTN_RES) == HIGH) {
        Maquina = Pantalla4;
        lcd.clear();
      }
      break;
    case Espera_Pantalla_INIT:
      if (digitalRead(Sensor_Puerta) == LOW) {
        lcd.clear();
        Maquina = Door;
        Puerta = Espera_Pantalla_INIT;
      }
      if (digitalRead(BTN_CHANGE) == HIGH) {
        lcd.clear();
        Maquina = INIT_TEMPERATURA;
      }
      break;
    case Door:
      Buzzer_Maquina();
      lcd.setCursor(0, 0);
      lcd.print("Hay una puerta");
      lcd.setCursor(0, 1);
      lcd.print("abierta");
      if (digitalRead(Sensor_Puerta) == LOW) {
        if (Puerta = INIT_HUMEDAD) {
          Maquina = INIT_HUMEDAD;
          lcd.clear();
        }
        if (Puerta = INIT_TEMPERATURA) {
          Maquina = INIT_TEMPERATURA;
          lcd.clear();
        }
        if (Puerta = Espera_Pantalla2) {
          Maquina = Espera_Pantalla2;
          lcd.clear();
        }
        if (Puerta = Pantalla2) {
          Maquina = Pantalla2;
          lcd.clear();
        }
        if (Puerta = Luz_SUM) {
          Maquina = Luz_SUM;
          lcd.clear();
        }
        if (Puerta = Espera_Pantalla3) {
          Puerta = Luz_SUM;
          lcd.clear();
        }
        if (Puerta = Pantalla3) {
          Maquina = Pantalla3;
          lcd.clear();
        }
        if (Puerta = Ventilador_Up) {
          Maquina = Ventilador_Up;
          lcd.clear();
        }
        if (Puerta = Ventilador_Down) {
          Maquina = Ventilador_Down;
          lcd.clear();
        }
        if (Puerta = Espera_Pantalla4) {
          Maquina = Espera_Pantalla4;
          lcd.clear();
        }
        if (Puerta = Pantalla4) {
          Maquina = Pantalla4;
          lcd.clear();
        }
        if (Puerta = Sum_Leds) {
          Maquina = Sum_Leds;
          lcd.clear();
        }
        if (Puerta = Rest_Leds) {
          Maquina = Rest_Leds;
          lcd.clear();
        }
        if (Puerta = Espera_Pantalla_INIT) {
          Maquina = Espera_Pantalla_INIT;
          lcd.clear();
        }
        if (Puerta = Espera_T_Eeprom) {
          Maquina = Espera_T_Eeprom;
          lcd.clear();
        }
        if (Puerta = T_Eeprom) {
          Maquina = T_Eeprom;
          lcd.clear();
        }
        if (Puerta = Sub_Eeprom) {
          Maquina = Sub_Eeprom;
          lcd.clear();
        }
        if (Puerta = Baj_Eeprom) {
          Maquina = Baj_Eeprom;
          lcd.clear();
        }
      }
  }
}