/*
 * SPI_LCD.c
 *
 *  Created on: 27 oct. 2024
 *      Author: Melissa
 */

#include "SPI_LCD.h"

void SPI_Config(void)
{
	// SPI CLK and MOSI
	GPIO_configPIN(GPIOF, 7, AF, PUSHP, VHIGH, NOPUPD, 5);
	GPIO_configPIN(GPIOF, 9, AF, PUSHP, VHIGH, NOPUPD, 5);

	RCC->APB2ENR |= BIT20;					// CLK Enable SPI5
	SPI5 -> CR1 |= BIT9 | BIT8 | BIT2;		// SSM, SSI, MSTR
	SPI5 -> CR1 |= BIT6;					// SPI Enabled
	// Baud Rate 18 MHz = 72 MHz /2 /2
}

void SPI_Transmit(uint8_t *data, uint32_t size)
{
	for(uint32_t i = 0; i<size; i++)
	{
		while(!(SPI5->SR & BIT1));			// Wait until transmit buffer empty
		SPI5->DR = data[i];					// Write data in DR
	}

	while(!(SPI5->SR & BIT1));			// Wait until transmit buffer empty
	while(SPI5->SR & BIT7);				// Wait until SPI not busy

	// Clear OVR flag
	(void) SPI5 -> DR;
	(void) SPI5 -> SR;
}
