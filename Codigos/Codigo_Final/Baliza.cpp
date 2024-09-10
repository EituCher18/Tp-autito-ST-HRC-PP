#include "Declaraciones.h"

void Baliza() {

  if (Leds = 0) {
    digitalWrite(Led_A1, LOW);
    digitalWrite(Led_A2, LOW);
  }

  if (Leds = 1) {
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