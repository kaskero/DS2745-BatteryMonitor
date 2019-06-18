#include <Wire.h>
#include <DS2745_lib.h>

DS2745_lib neurgailua;

void setup() {
  Serial.begin(9600);

  Wire.begin();

  neurgailua.abiarazi();
}

void loop() {
  float tentsioa = neurgailua.tentsioaIrakurri();
  float kontsumoa = neurgailua.kontsumoaIrakurri();

  delay(3500); // Current conversion time 3.5s
}




