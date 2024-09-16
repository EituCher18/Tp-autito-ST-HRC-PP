#define BTN1
#define BTN2
#define BTN3
#define Puerta
#include <Arduino.h>
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(BTN1, INPUT_PULLUP);
pinMode(BTN2, INPUT_PULLUP);
pinMode(BTN3, INPUT_PULLUP);
pinMde(Puerta, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
Serial.print("BTN1      ");
Serial.print(digitalRead(BTN1));
Serial.print("        BTN2   ");
Serial.print(digitalRead(BTN2));
Serial.print("     BTN3     ");
Serial.print(digitalRead(BTN3));
Serial.print("    Puerta     ");
Serial.println(digitalRead(Puerta));
}
