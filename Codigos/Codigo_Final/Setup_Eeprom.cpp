#include "Declaraciones.h"
void SetupEeprom() {
  preferences.begin("my-app", false);
  Intervalo = preferences.getInt("Intervalo_Eeprom", 30000);
  Umb_Lum = preferences.getInt("Umb_Lum_Eeprom", 500);
}