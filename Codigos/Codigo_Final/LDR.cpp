#include "Declaraciones.h"
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