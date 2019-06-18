/*
  DS2745_lib.cpp - Library for using DS2745
  Created by Oskar Casquero and Ander Abadia, May 22, 2018.
  Released into the public domain.
*/
#include "DS2745_lib.h"
#include "Wire.h"

DS2745_lib::DS2745_lib() {}
	
void DS2745_lib::abiarazi() {
  Wire.beginTransmission(DS2745_I2C_ADDRESS);
  Wire.write(STATUS_REG); // Punteroa erregistroaren 0x01 helbidean kokatu (Status reg)
  Wire.endTransmission();

  // Wire.requestFrom() waits until the I2C transaction has finished
  Wire.requestFrom(DS2745_I2C_ADDRESS, 1);
  byte status_reg = Wire.read();
  byte porf = (status_reg & 0x40) >> 6; // PORF bit
  if(porf) { // PORF is 1
    Wire.beginTransmission(DS2745_I2C_ADDRESS);
    Wire.write(STATUS_REG); // Punteroa erregistroaren 0x01 helbidean kokatu (Status reg)
    Wire.write(status_reg & ~(0x40));
    Wire.endTransmission();
  }
}
	
float DS2745_lib::tentsioaIrakurri() {
  Wire.beginTransmission(DS2745_I2C_ADDRESS);
  Wire.write(VOLTAGE_REG_MSB); //Punteroa erregistroaren 0x0C helbidean kokatu (Voltage MSB reg)
  Wire.endTransmission();

  // Wire.requestFrom() waits until the I2C transaction has finished
  int Vreceived = Wire.requestFrom(DS2745_I2C_ADDRESS, 2);
  byte MSBvoltage = Wire.read(); 
  byte sign = MSBvoltage >> 7;
  MSBvoltage = MSBvoltage & 0x7F;
  byte LSBvoltage = Wire.read(); 
   
  // LSB-aren less significant 5 bitak erreserbatuta daude. 
  unsigned int voltage_ADC = ((MSBvoltage << 8) | LSBvoltage) >> 5; 

  float voltage = (voltage_ADC * 4.88) / 1000;
  if(sign) return -voltage;
  else return voltage;
}

float DS2745_lib::kontsumoaIrakurri() {
  Wire.beginTransmission(DS2745_I2C_ADDRESS);
  Wire.write(CURRENT_REG_MSB); //Punteroa erregistroaren 0x0E helbidean kokatu (Current MSB reg)
  Wire.endTransmission();  

  // Wire.requestFrom() waits until the I2C transaction has finished
  int Ireceived = Wire.requestFrom(DS2745_I2C_ADDRESS, 2);

  byte MSBcurrent = Wire.read(); 
  byte sign = MSBcurrent >> 7;
  MSBcurrent = MSBcurrent & 0x7F;
  byte LSBcurrent = Wire.read(); 

  unsigned int current_ADC = ((MSBcurrent << 8) | LSBcurrent); 
  if(sign) current_ADC = 0x7FFF - current_ADC;

  float current = current_ADC * 0.0625; //current = current_ADC * 0.0625; // Units: 1.5625uV/Rsns --> 1.5625uV/0.025ohm=62.5uA
  if(sign) current = -current;

  return current; 
}