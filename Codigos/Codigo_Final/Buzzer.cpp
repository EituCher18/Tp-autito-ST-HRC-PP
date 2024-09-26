#include "Declaraciones.h"
void Buzzer_Maquina() {
  switch (Maquina_Buzzer) {
    case Buzzer_On:
      digitalWrite(BUZZER_PIN, HIGH);
      if (millis() - Ms_Buzzer >= 1000) {
        Ms_Buzzer = millis();
        Maquina_Buzzer = Buzzer_Off;
      }
      break;
    case Buzzer_Off:
      digitalWrite(BUZZER_PIN, LOW);
      if (millis() - Ms_Buzzer >= 1000) {
        Ms_Buzzer = millis();
        Maquina_Buzzer = Buzzer_On;
      }
      break;
  }
}