/*
  DS2745_lib.h - Library for using DS2745
  Created by Oskar Casquero and Ander Abadia, May 22, 2018.
  Released into the public domain.
*/

#ifndef DS2745_lib_h
#define DS2745_lib_h

#include "Arduino.h"

#define DS2745_I2C_ADDRESS B1001000

#define STATUS_REG 0x01
#define VOLTAGE_REG_MSB 0x0C
#define CURRENT_REG_MSB 0x0E

class DS2745_lib {
  public:
    DS2745_lib();
	void abiarazi();
	float tentsioaIrakurri();
	float kontsumoaIrakurri();
};

#endif