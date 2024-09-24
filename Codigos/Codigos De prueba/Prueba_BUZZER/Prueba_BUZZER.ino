// Incluir la librería EasyBuzzer
#include <EasyBuzzer.h>
#include <Arduino.h>
// Definir el pin donde está conectado el buzzer
#define BUZZER_PIN 26

void setup() {
  // Establecer el pin del buzzer
  EasyBuzzer.setPin(BUZZER_PIN);
}

void loop() {
  // Actualizar el estado del buzzer
  EasyBuzzer.update();

  // Hacer un beep simple a 1000 Hz por 500 ms
  EasyBuzzer.singleBeep(1000, 1000);

  // // Esperar un segundo
  // delay(1000);

  // // Hacer una secuencia de beeps a 2000 Hz por 100 ms cada uno, con una pausa de 50 ms entre ellos, y repetir la secuencia 3 veces
  // EasyBuzzer.beep(2000, 100, 50, 5, 200, 3);
  //Lo primero es a cuantos Hz va a sonar, lo segundo es la duracion de cada uno, lo tercero es la duracion de apagado, el 4 es cuantas veces se repite el sonido y el silencio, lo 5 es la duracion del silencio, y lo ultimo es las veces que se repite esta secuencia
}
