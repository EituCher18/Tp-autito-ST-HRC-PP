#ifndef DECLARACIONES_H
#define DECLARACIONES_H

#include "Arduino.h"

//Librerias
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <AHT10.h>  // Librería para utilizar el sensor AHT10
#include <Preferences.h>
Preferences preferences;
//Variables de tiempo
unsigned long Ms;
unsigned long Ms_Baliza;
unsigned long Ms_Eeprom;
unsigned long Ms_Buzzer;


//Pines

#define BTN_CHANGE 12
#define BTN_SUM 12
#define BTN_RES 12
#define Led_A1 12
#define Led_A2 12
#define Led_B1 12
#define Led_B2 12
#define LDR 12
#define Rele 12
#define Cooler 12
#define BUZZER_PIN 12
#define Sensor_Puerta 12
// AHT10
AHT10 myAHT10(0x38);

// LCD
extern uint8_t lcdAddress = 0x27;
LiquidCrystal_I2C lcd(lcdAddress, 16, 2);

extern uint8_t Maquina;
extern uint8_t Leds;
extern uint8_t Umb_Lum;
extern uint8_t Ventilador;
extern uint8_t Maquina_Baliza;
extern uint8_t Puerta;
extern uint8_t Intervalo;
extern uint8_t Intervalo_Corto;
extern uint8_t Maquina_Buzzer;
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


void setupLibrerias();

void Baliza();

void Eeprom();

void Venti();

void S_Luz();

void SetupContadores();

void SetupEeprom();

void pines();

void Buzzer_Maquina();
#endif