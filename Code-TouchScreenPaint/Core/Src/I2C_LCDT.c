/*
 * I2C_LCDT.c
 *
 *  Created on: 27 nov. 2024
 *      Author: Melissa
 *      Based on the codes of https://blog.embeddedexpert.io and https://stm32f4-discovery.net
 */

#include "I2C_LCDT.h"

void I2C_init(void)
{
	// GPIO Configuration
	GPIO_configPIN(GPIOA, 8, AF, OPEND, MEDIUM, PUP, 4); // SCL
	GPIO_configPIN(GPIOC, 9, AF, OPEND, MEDIUM, PUP, 4); // SDA

	// I2C3 Standard Mode (SM) 100 kHz
	RCC->APB1ENR |= BIT23;		// CLK Enable I2C3

	I2C3->CR1 = BIT15;			// Reset
	I2C3->CR1 &=~ BIT15;		// Release reset

	I2C3->CR2 |= 0x12;			// Set CLK 18MHz	= 72 MHz/4
	I2C3->CCR = 0x5A;  			// CCR = 18 MHz/(2*100 kHz)
	I2C3->TRISE = 0x13; 		// Output max rise CLK+1

	I2C3->CR1 |= BIT0; 			// Enable I2C
}


void I2C_writeByte(uint8_t saddr,uint8_t maddr, uint8_t data)
{
	while(I2C3->SR2 & BIT1);          		 // Wait until bus not busy
	I2C3->CR1 |= BIT8;                 		 // Generate start
	while(!(I2C3->SR1 & BIT0));              // Wait until start bit is set

	I2C3->DR = saddr<<1;                 	 // Send slave address
	while(!(I2C3->SR1 & BIT1));     	     // Wait until address flag is set
	(void)I2C3->SR2; 						 // Clear SR2 by reading it

	while(!(I2C3->SR1 & BIT7));            	 // Wait until Data register empty
	I2C3->DR = maddr;                     	 // Send memory address
	while(!(I2C3->SR1 & BIT7));              // Wait until data register empty

	I2C3->DR = data;						 // Write Data
	while (!(I2C3->SR1 & BIT2));      		 // Wait until transfer finished
	I2C3->CR1 |= BIT9;						 // Generate Stop
}

uint8_t I2C_readByte(uint8_t saddr, uint8_t maddr)
{
	while(I2C3->SR2 & BIT1);          		 // Wait until bus not busy
	I2C3->CR1 |= BIT8;                 		 // Generate start
	while(!(I2C3->SR1 & BIT0));              // Wait until start bit is set

	I2C3->DR = saddr<<1;                 	 // Send slave address
	while(!(I2C3->SR1 & BIT1));     	     // Wait until address flag is set
	(void)I2C3->SR2; 						 // Clear SR2 by reading it

	while(!(I2C3->SR1 & BIT7));            	 // Wait until data transmitter register empty
	I2C3->DR = maddr;                     	 // Send memory address
	while(!(I2C3->SR1 & BIT7));              // Wait until data transmitter register empty

	I2C3->CR1 |= BIT8;                 		 // Generate repeated start
	while(!(I2C3->SR1 & BIT0));              // Wait until start bit is set

    I2C3->DR = saddr << 1 | 1;               // Send slave address with read
	while(!(I2C3->SR1 & BIT1));     	     // Wait until address flag is set
	(void)I2C3->SR2; 						 // Clear SR2 by reading it

    I2C3->CR1 &= ~BIT10;                	 // No acknowledge
    I2C3->CR1 |= BIT9;						 // Generate Stop

    while(!(I2C3->SR1 & BIT6));              // Wait until data receiver register empty
    return I2C3->DR;                        // read data from DR
}


void I2C_readMultiByte(uint8_t saddr,uint8_t maddr, uint8_t* data, uint16_t n)
{
	while(I2C3->SR2 & BIT1);          		 // Wait until bus not busy
	I2C3->CR1 |= BIT8;                 		 // Generate start
	while(!(I2C3->SR1 & BIT0));              // Wait until start bit is set

	I2C3->DR = saddr<<1;                 	 // Send slave address
	while(!(I2C3->SR1 & BIT1));     	     // Wait until address flag is set
	(void)I2C3->SR2; 						 // Clear SR2 by reading it

	while(!(I2C3->SR1 & BIT7));            	 // Wait until data transmitter register empty
	I2C3->DR = maddr;                     	 // Send memory address
	while(!(I2C3->SR1 & BIT7));              // Wait until data transmitter register empty

	I2C3->CR1 |= BIT8;                 		 // Generate repeated start
	while(!(I2C3->SR1 & BIT0));              // Wait until start bit is set

	I2C3->DR = saddr << 1 | 1;               // Send slave address with read
	while(!(I2C3->SR1 & BIT1));     	     // Wait until address flag is set
	(void)I2C3->SR2; 						 // Clear SR2 by reading it

	I2C3->CR1 |= BIT10;                  	 // Acknowledge
	while(n>0U)
	{
		if(n==1U)
		{
			I2C3->CR1&=~I2C_CR1_ACK;
			I2C3->CR1|=I2C_CR1_STOP;
			while(!(I2C3->SR1&I2C_SR1_RXNE)){;}
			*data++=I2C3->DR;
			break;
		}
		else
		{
			while(!(I2C3->SR1&I2C_SR1_RXNE)){;}
			(*data++)=I2C3->DR;
			n--;
		}
	}
}
