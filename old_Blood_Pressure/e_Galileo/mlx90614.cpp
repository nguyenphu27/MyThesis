/*
 * mlx90614.cpp
 *
 *  Created on: Oct 31, 2016
 *      Author: phu
 */

#include "mlx90614.h"
#include <Wire.h>

void MLX90614::wireBegin() {
	Wire.begin();
}

double MLX90614::readObjectTemperature() {
	return readTemp(MLX90614_OBJECT);
}

double MLX90614::readAmbientTemperature() {
	return readTemp(MLX90614_AMBIENT);
}

float MLX90614::readTemp(uint8_t mode) {
	float temp;
	temp = readI2C(mode);
	temp *= .02;
	temp -= 273.2;

	return temp;
}

uint16_t MLX90614::readI2C(uint8_t mode) {
	uint16_t result;

	Wire.beginTransmission(MLX90614_ADDR);
	Wire.write(mode);
	Wire.endTransmission(false);

	Wire.requestFrom(MLX90614_ADDR, (int) 3);
	result = Wire.read();
	result |= Wire.read() << 8;

	return result;
}
