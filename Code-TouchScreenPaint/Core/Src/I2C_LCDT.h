/*
 * I2C_LCDT.h
 *
 *  Created on: 27 nov. 2024
 *      Author: Melissa
 *      Based on the codes of https://blog.embeddedexpert.io and https://stm32f4-discovery.net
 */

#ifndef I2C_LCDT_H_
#define I2C_LCDT_H_

/* Includes */
#include "stm32f4xx.h"
#include "GPIO.h"

void I2C_init(void);
void I2C_writeByte(uint8_t saddr,uint8_t maddr, uint8_t data);
uint8_t I2C_readByte(uint8_t saddr, uint8_t maddr);
void I2C_readMultiByte(uint8_t saddr,uint8_t maddr, uint8_t *data, uint16_t n);

#endif /* I2C_LCDT_H_ */
