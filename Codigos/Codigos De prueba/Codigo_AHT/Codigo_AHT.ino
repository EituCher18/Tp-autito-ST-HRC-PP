#include <AHTxx.h>
#include <Wire.h>
#include <Arduino.h>

float AHT_Tem;
float AHT_Hum;
AHTxx aht10(AHTXX_ADDRESS_X38, AHT1x_SENSOR);



void Temperatura_Y_Humedad();

void Temperatura();

void Humedad();


void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  while (aht10.begin() != true) {
    Serial.println("Fallo en conectar");

    delay(5000);
  }
  Serial.println("Se conecto");
  
}

void loop() {
  Temperatura_Y_Humedad();
}


void Humedad() {
  AHT_Hum = aht10.readHumidity();
  Serial.print("La Humedad Es          ");
  Serial.println(AHT_Hum);
}

void Temperatura() {

  AHT_Tem = aht10.readTemperature();
  Serial.print("La temperatura es     ");
  Serial.println(AHT_Tem);
}

void Temperatura_Y_Humedad() {
delay(500);
  AHT_Tem = aht10.readTemperature();
  Serial.print("La temperatura es     ");
  Serial.print(AHT_Tem);
  AHT_Hum = aht10.readHumidity();
  Serial.print("       La Humedad Es          ");
  Serial.println(AHT_Hum);
}
