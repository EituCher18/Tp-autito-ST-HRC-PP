//Grupo 8 Eitan Cherniak, Uriel Digestani, Aaron Yabra
#include "Arduino.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <AHT10.h>  // Librería para utilizar el sensor AHT10
#include <Preferences.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <UniversalTelegramBot.h>
#include <WiFi.h>



//Cosas del bot de telegram
#define BOTtoken "  "
#define CHAT_ID "   "
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);


///WiFi
#define WIFI_SSID "ORT-IoT"
#define WIFI_PASSWORD "OrtIOT24"


Preferences preferences;
///Variables de tiempo
unsigned long Ms;
unsigned long Ms_Baliza;
unsigned long Ms_Eeprom;
unsigned long Ms_Buzzer;



///Pines
#define BTN_SUM 10
#define BTN_RES 9
#define Led_A1 25
#define Led_A2 23
#define Led_B1 24
#define Led_B2 22
#define LDR 11
#define Rele 13
#define Cooler 12
#define BUZZER_PIN 21
#define Sensor_Puerta 7


// AHT10
AHT10 myAHT10(0x38);

/// LCD
uint8_t lcdAddress = 0x27;
LiquidCrystal_I2C lcd(lcdAddress, 16, 2);

uint8_t Maquina;
uint8_t Leds;
uint8_t Umb_Lum;
uint8_t Ventilador;
uint8_t Maquina_Baliza;
uint8_t Puerta;
uint8_t Intervalo;
uint8_t Intervalo_Corto;
uint8_t Maquina_Buzzer;
uint8_t Verifi_Puerta = 0;
uint8_t Verifi_Luz = 0;
enum Estados {
  INIT_HUMEDAD,
  INIT_TEMPERATURA,
  Espera_Pantalla2,
  Pantalla2,
  Luz_SUM,
  Espera_Pantalla3,
  Pantalla3,
  Ventilador_Up,
  Ventilador_Down,
  Espera_Pantalla4,
  Pantalla4,
  Sum_Leds,
  Rest_Leds,
  Espera_Pantalla_INIT,
  Door,
  Espera_T_Eeprom,
  T_Eeprom,
  Sub_Eeprom,
  Baj_Eeprom,
  Luz_Baj,
};
enum Baliza {
  On,
  Off,
};

enum Door {
  Buzzer_On,
  Buzzer_Off
};

// Nucleos
TaskHandle_t Core0Task;
TaskHandle_t Core1Task;


void setupLibrerias();

void Baliza();

void Eeprom();

void Venti();

void S_Luz();

void SetupContadores();

void SetupEeprom();

void pines();

void Buzzer_Maquina();

void Setup_Nucleos();

void codeForCore0Task(void* parameter);

void codeForCore1Task(void* parameter);

void handleNewMessages(int numNewMessages);
void initWiFi();


void Venti() {
  if (Ventilador == 1) {
    digitalWrite(Cooler, HIGH);
  }
  if (Ventilador == 0) {
    digitalWrite(Cooler, LOW);
  }
}
void S_Luz() {
  if (analogRead(LDR) >= Umb_Lum) {
    digitalWrite(Led_B1, HIGH);
    digitalWrite(Led_B2, HIGH);
  }
  if (analogRead(LDR) <= Umb_Lum) {
    digitalWrite(Led_B1, LOW);
    digitalWrite(Led_B2, LOW);
  }
}
void SetupContadores() {
  Ms = 0;
  Ventilador = 0;
  Leds = 0;
  Ms_Baliza = 0;
  Ms_Eeprom = 0;
}
void SetupEeprom() {
  preferences.begin("my-app", false);
  Intervalo = preferences.getInt("Intervalo_Eeprom", 30000);
  Umb_Lum = preferences.getInt("Umb_Lum_Eeprom", 500);
}
void Eeprom() {
  if (millis() - Ms_Eeprom >= Intervalo) {
    Ms_Eeprom = millis();
    Intervalo_Corto = Intervalo / 1000;
    preferences.putInt("Intervalo_Eeprom", Intervalo);
    preferences.putInt("Umb_Lum_Eeprom", Umb_Lum);
  }
}
void pines() {
  pinMode(BTN_SUM, INPUT_PULLUP);
  pinMode(BTN_RES, INPUT_PULLUP);
  pinMode(Led_A1, OUTPUT);
  pinMode(Led_A2, OUTPUT);
  pinMode(Led_B1, OUTPUT);
  pinMode(Led_B2, OUTPUT);
  pinMode(Sensor_Puerta, INPUT_PULLUP);
  pinMode(Cooler, INPUT_PULLUP);
  pinMode(LDR, INPUT);
  pinMode(Rele, OUTPUT);
}
void Buzzer_Maquina() {
  switch (Maquina_Buzzer) {
    case Buzzer_On:
      digitalWrite(BUZZER_PIN, HIGH);
      if (millis() - Ms_Buzzer >= 20000) {
        Ms_Buzzer = millis();
        Maquina_Buzzer = Buzzer_Off;
      }

      break;
    case Buzzer_Off:
      digitalWrite(BUZZER_PIN, LOW);
      if (millis() - Ms_Buzzer >= 20000) {
        Ms_Buzzer = millis();
        Maquina_Buzzer = Buzzer_On;
      }
      break;
  }
}
void Setup_Nucleos() {
  xTaskCreatePinnedToCore(
    codeForCore0Task,
    "Core 0 task",
    10000,
    NULL,
    1,
    &Core0Task,
    0);
  xTaskCreatePinnedToCore(
    codeForCore1Task,
    "Core 1 task",
    10000,
    NULL,
    1,
    &Core1Task,
    1);
}
void setupLibrerias() {
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
void setup() {
  // put your setup code here, to run once:
  initWiFi();
  setupLibrerias();
  SetupContadores();
  SetupEeprom();
  pines();
  Setup_Nucleos();
  Maquina = INIT_HUMEDAD;
  Maquina_Baliza = On;
}
void loop() {
}
void codeForCore0Task(void* parameter) {
  for (;;) {
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
        if (digitalRead(BTN_RES) == LOW && digitalRead(BTN_SUM) == LOW) {
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
        if (digitalRead(BTN_RES) == LOW && digitalRead(BTN_SUM) == LOW) {
          Maquina = Espera_Pantalla2;
        }
        break;

      case Espera_Pantalla2:
        if (digitalRead(Sensor_Puerta) == LOW) {
          lcd.clear();
          Maquina = Door;
          Puerta = Espera_Pantalla2;
        }
        if (digitalRead(BTN_RES) == HIGH == HIGH && digitalRead(BTN_SUM) == HIGH) {
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
        }
        if (digitalRead(BTN_RES) == LOW) {
          Maquina = Luz_Baj;
        }
        if (digitalRead(BTN_RES) == LOW && digitalRead(BTN_SUM) == LOW) {
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
          Umb_Lum = Umb_Lum - 100;
        }
        if (digitalRead(BTN_RES) == LOW && digitalRead(BTN_SUM) == LOW) {
          Maquina = Espera_Pantalla3;
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
          Umb_Lum = Umb_Lum + 100;
        }
        if (digitalRead(BTN_RES) == LOW && digitalRead(BTN_SUM) == LOW) {
          Maquina = Espera_Pantalla3;
        }
        break;
      case Espera_Pantalla3:
        if (digitalRead(Sensor_Puerta) == LOW) {
          lcd.clear();
          Maquina = Door;
          Puerta = Espera_Pantalla3;
        }
        if (digitalRead(BTN_RES) == HIGH && digitalRead(BTN_SUM) == HIGH) {
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
          Maquina = Ventilador_Up;
        }
        if (digitalRead(BTN_RES) == LOW) {
          Maquina = Ventilador_Down;
        }
        if (digitalRead(BTN_RES) == LOW && digitalRead(BTN_SUM) == LOW) {
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
          Ventilador = 0;
        }
        if (digitalRead(BTN_RES) == LOW && digitalRead(BTN_SUM) == LOW) {
          Maquina = Espera_T_Eeprom;
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
          Ventilador = 1;
        }
        if (digitalRead(BTN_RES) == LOW && digitalRead(BTN_SUM) == LOW) {
          Maquina = Espera_T_Eeprom;
        }
        break;
      case Espera_Pantalla4:
        if (digitalRead(Sensor_Puerta) == LOW) {
          lcd.clear();
          Maquina = Door;
          Puerta = Espera_Pantalla4;
        }
        if (digitalRead(BTN_RES) == HIGH && digitalRead(BTN_SUM) == HIGH) {
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
        if (digitalRead(BTN_RES) == HIGH && digitalRead(BTN_SUM) == HIGH) {
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
        if (digitalRead(BTN_RES) == LOW && digitalRead(BTN_SUM) == LOW) {
          Maquina = Espera_Pantalla4;
        }

        if (digitalRead(BTN_SUM == LOW)) {

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
          Intervalo++;
        }
        if (digitalRead(BTN_RES) == LOW && digitalRead(BTN_SUM) == LOW) {
          Maquina = Espera_Pantalla4;
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
          Intervalo--;
        }
        if (digitalRead(BTN_RES) == LOW && digitalRead(BTN_SUM) == LOW) {
          Maquina = Espera_Pantalla4;
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
        if (digitalRead(BTN_RES) == LOW && digitalRead(BTN_SUM) == LOW) {
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

          Maquina = Sum_Leds;
        }
        if (digitalRead(BTN_RES) == LOW)

        {

          Maquina = Rest_Leds;
        }
        break;
      case Sum_Leds:
        if (digitalRead(Sensor_Puerta) == LOW) {
          lcd.clear();
          Maquina = Door;
          Puerta = Sum_Leds;
        }
        if (digitalRead(BTN_SUM) == HIGH) {
          Maquina = Pantalla4;
          lcd.clear();
          Leds = 1;
        }
        if (digitalRead(BTN_RES) == LOW && digitalRead(BTN_SUM) == LOW) {
          Maquina = Espera_Pantalla_INIT;
        }
        break;
      case Rest_Leds:
        if (digitalRead(BTN_RES) == LOW && digitalRead(BTN_SUM) == LOW) {
          Maquina = Espera_Pantalla_INIT;
        }
        if (digitalRead(Sensor_Puerta) == LOW) {
          lcd.clear();
          Maquina = Door;
          Puerta = Rest_Leds;
        }
        if (digitalRead(BTN_RES) == HIGH) {
          Maquina = Pantalla4;
          lcd.clear();
          Leds = 0;
        }
        break;
      case Espera_Pantalla_INIT:
        if (digitalRead(Sensor_Puerta) == LOW) {
          lcd.clear();
          Maquina = Door;
          Puerta = Espera_Pantalla_INIT;
        }
        if (digitalRead(BTN_RES) == HIGH && digitalRead(BTN_SUM) == HIGH) {
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
}
void codeForCore1Task(void* parameter) {
  uint8_t numNewMessages;
  uint8_t stateMessage = 0;
  uint8_t Bot_lasttime;
  for (;;) {
    if (millis() > Bot_lasttime + 1000) {
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
      while (numNewMessages) {
        Serial.println("got response");
        handleNewMessages(numNewMessages);
        numNewMessages = bot.getUpdates(bot.last_message_received + 1);
      }
      Bot_lasttime = millis();
    }
  }
}

void handleNewMessages(int numNewMessages) {
  String text = "";
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Guest";

    if (text == "/Esta_Prendido?") {
      bot.sendMessage(chat_id, "Si  https://youtu.be/xvFZjo5PgG0");
    }

    if (text == "/Temperatura_actual") {
      String Mesagge_Temp = "Temperatura actual: " + String(myAHT10.readTemperature()) + "°C";
      bot.sendMessage(chat_id, Mesagge_Temp);
    }
    if (text == "/Humedad_actual") {
      String Mesagge_Hum = "La Humdad actual: " + String(myAHT10.readHumidity()) + " %";
      bot.sendMessage(chat_id, Mesagge_Hum);
    }

    if (text == "/La_baliza_esta_prendida?") {
      if (Leds == 1) {
        bot.sendMessage(chat_id, "La baliza esta prendida.");
      } else {
        bot.sendMessage(chat_id, "La baliza esta apagada.");
      }
    }
    if (text == "/Encender_baliza") {
      Leds = 1;
      bot.sendMessage(chat_id, "Se encendio la baliza");
    }
    if (text == "/Apagar_baliza") {
      Leds = 0;
      bot.sendMessage(chat_id, "Se apago la baliza");
    }
    if (text == "/umbral") {
      i++;
      text = bot.messages[i].text;
      Umb_Lum = text.toInt();
    }
    if (text == "/ventilador") {
      Ventilador = !Ventilador;
    }
    if (text == "/buzzer") {
      if (Maquina_Buzzer == Buzzer_On) {
        Maquina_Buzzer = Buzzer_Off;
        digitalWrite(BUZZER_PIN, LOW);
        Ms_Buzzer = millis();
      } else {
        Maquina_Buzzer = Buzzer_On;
        digitalWrite(BUZZER_PIN, HIGH);
        Ms_Buzzer = millis();
      }
    }

    if (Maquina == Door && Verifi_Puerta == 0) {
      bot.sendMessage(chat_id, "Se abrio la puerta");
      Verifi_Puerta = 1;
    }
    if (Maquina != Door && Verifi_Puerta == 1) {
      bot.sendMessage(chat_id, "Se cerro la puerta");
      Verifi_Puerta = 0;
    }
    if (analogRead(LDR) >= Umb_Lum && Verifi_Luz == 0) {
      bot.sendMessage(chat_id, "Se encendieron las luces de iluminacion");
      Verifi_Luz = 1;
    }
    if (analogRead(LDR) < Umb_Lum) {
      Verifi_Luz = 0;
    }
  }
}

void Baliza() {
  if (Leds == 0) {
    digitalWrite(Led_A1, LOW);
    digitalWrite(Led_A2, LOW);
  }
  if (Leds == 1) {
    switch (Maquina_Baliza) {
      case On:
        digitalWrite(Led_A1, HIGH);
        digitalWrite(Led_A2, HIGH);

        if (millis() - Ms_Baliza >= 1000) {
          Maquina_Baliza = Off;
          Ms_Baliza = millis();
        }
        break;

      case Off:
        digitalWrite(Led_A1, LOW);
        digitalWrite(Led_A2, LOW);
        if (millis() - Ms_Baliza >= 1000) {
          Maquina_Baliza = On;
          Ms_Baliza = millis();
        }
        break;
    }
  }
}

void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
    lcd.setCursor(0,0);
    lcd.print("Conectando WIFI");

  }
  Serial.println(WiFi.localIP());
  Serial.println();
}
