#define LDR 13
#include <Arduino.h>

void setup() {
  // put your setup code here, to run once:

pinMode(LDR, INPUT);
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
Serial.print("La lectura de LDR es    ");
Serial.println(analogRead(LDR));
}