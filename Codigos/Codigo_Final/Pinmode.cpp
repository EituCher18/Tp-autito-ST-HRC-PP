#include "Declaraciones.h"
void pines(){
  pinMode(BTN_CHANGE, INPUT_PULLUP);
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