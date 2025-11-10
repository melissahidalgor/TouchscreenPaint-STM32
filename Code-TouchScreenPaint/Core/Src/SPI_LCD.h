/*
 * SPI_LCD.h
 *
 *  Created on: 27 oct. 2024
 *      Author: Melissa
 *
 * SPI 5 for LCD TFT
 */

#ifndef SPI_LCD_H_
#define SPI_LCD_H_

// Includes
#include "stm32f4xx.h"
#include "macros_utiles.h"
#include "GPIO.h"

void SPI_Config(void);
void SPI_Transmit(uint8_t *data, uint32_t size);

#endif /* SPI_LCD_H_ */
