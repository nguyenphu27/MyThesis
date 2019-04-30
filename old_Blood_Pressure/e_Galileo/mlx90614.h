/*
 * mlx90614.h
 *
 *  Created on: Oct 31, 2016
 *      Author: phu
 */

#ifndef HEALTHLIBS_MLX90614_H_
#define HEALTHLIBS_MLX90614_H_

#include <Arduino.h>

#define MLX90614_ADDR 0x5A
#define MLX90614_AMBIENT 0x06
#define MLX90614_OBJECT 0x07

class MLX90614 {

public:
	void wireBegin();
	double readObjectTemperature();
	double readAmbientTemperature();
private:
	float readTemp(uint8_t mode);
	uint16_t readI2C(uint8_t mode);
};

#endif /* HEALTHLIBS_MLX90614_H_ */
