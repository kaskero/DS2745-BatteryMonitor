#include <Wire.h>
#include <DS2745_lib.h>
#include "U8glib.h"

U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE);  // I2C / TWI

DS2745_lib neurgailua;

void setup() {
  Serial.begin(9600);

  Wire.begin();

  neurgailua.abiarazi();

  u8g.setFont(u8g_font_fub17);
}

void loop() {
  float tentsioa = neurgailua.tentsioaIrakurri();
  float kontsumoa = neurgailua.kontsumoaIrakurri();

  delay(3500); // Current conversion time 3.5s

  datuakBistaratu(tentsioa, kontsumoa);
}

void datuakBistaratu(float tentsioa, float kontsumoa) {
  u8g.firstPage();  
  do {
    u8g.setPrintPos(10, 30);
    u8g.print(tentsioa, 2);
    u8g.print(" V");
    u8g.setPrintPos(10, 50);
    u8g.print(kontsumoa, 2);
    u8g.print(" mA");
  } while( u8g.nextPage() );
}
