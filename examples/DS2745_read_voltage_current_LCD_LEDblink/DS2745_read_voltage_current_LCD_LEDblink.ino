#include <Wire.h>
#include <DS2745_lib.h>
#include "U8glib.h"

U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE);  // I2C / TWI

volatile int cont = 0;
volatile float t = 0.0;
volatile bool flag = false;

DS2745_lib neurgailua;

void setup() {
  Serial.begin(9600);

  Wire.begin();

  neurgailua.abiarazi();

  u8g.setFont(u8g_font_fub17);

  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  
  set_timer2_interrupt();
}

void loop() {
  float tentsioa = neurgailua.tentsioaIrakurri();
  float kontsumoa = neurgailua.kontsumoaIrakurri();

  delay(3500); // Current conversion time 3.5s

  datuakBistaratu(tentsioa, kontsumoa);

  if(flag) {
    digitalWrite(13, !digitalRead(13));
    flag = false;
  }
}

void set_timer2_interrupt() {
  cli();
  
  // Timer2 clk/1024 --> t_BIT_TCNT2: 1 / (8*10^6 / 1024) = 128us
  // OCR2A? --> OCR2A * 128us = 1s --> OCR2A = 7812.5
  // OCR2A 8bit-eko erregistroa da... --> 7812.5/256 ~ 30.517 --> 1s
  //                                                   305.17 --> 10s
  
  // Table 20-8. Waveform Generation Mode Bit Description
  // Mode WGM22 WGM21 WGM20 Timer/Counter_Mode_of_Operation  TOP
  // 0    0     0     0     Normal                           0xFF
  
  // 20.10.1 TCCR2A – Timer/Counter2 Control Register A
  // COM2A1 COM2A0 COM2B1 COM2B0 X X WGM21 WGM20
  // Table 20-2. Compare Output Mode, non-PWM
  // COM2A1 COM2A0 Description
  // 0      0      Normal port operation, OC2A disconnected
  TCCR2A &= (~(_BV(COM2A1)) & ~(_BV(COM2A0)));
  TCCR2A &= (~(_BV(WGM21)) & ~(_BV(WGM20)));
  
  // 20.10.2 TCCR2B – Timer/Counter1 Control Register B
  // X X - - WGM22 CS22 CS21 CS20
  TCCR2B &= ~(_BV(WGM22));
  
  // Table 20-9. Clock Select Bit Description
  // CS22 CS21 CS20 Description
  // 1    1    1    clk/1024
  TCCR2B |= (_BV(CS22) | _BV(CS21) | _BV(CS20));

  TCNT2 = 0;
  TIMSK2 |= 1<<OCIE2A;
  
  sei();
}

ISR(TIMER2_COMPA_vect) {
  if(cont == 610) {
    long now = micros();
    Serial.print("Timer2 interrupt: "); Serial.println((now - t) / 1000000.0); Serial.println();
    t = now;
    cont = 0;
    flag = true;
  } else {
    cont++;
  }
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
