#include "Declaraciones.h"

void Eeprom() {
  if (millis() - Ms_Eeprom >= Intervalo) {
    Ms_Eeprom = millis();
    Intervalo_Corto = Intervalo / 1000;
    preferences.putInt("Intervalo_Eeprom", Intervalo);
    preferences.putInt("Umb_Lum_Eeprom", Umb_Lum);
  }
}