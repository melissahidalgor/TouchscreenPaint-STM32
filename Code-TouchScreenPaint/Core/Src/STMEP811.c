/*
 * STMEP811.c
 *
 *  Created on: 27 nov. 2024
 *      Author: Melissa
 *      Based on the codes of https://blog.embeddedexpert.io and https://stm32f4-discovery.net
 */

#include "STMEP811.h"


void STMPE811_Touch_Enable(void)
{
	TIM_config(1000);
	uint8_t mode;

	I2C_writeByte(deviceAddress, STMPE811_REG_SYS_CTRL1, 2); 	// Power Down
	TIM_delay(10);										 	// Delay to ensure registers erasing

	I2C_writeByte(deviceAddress, STMPE811_REG_SYS_CTRL1, 0); 	// Power On the Codec after the power off
	TIM_delay(2);										 	// Delay to ensure registers erasing

	mode = I2C_readByte(deviceAddress, STMPE811_REG_SYS_CTRL2);	// Disable clock access to GPIO
	mode &= ~(STMPE811_IO_FCT);
	I2C_writeByte(deviceAddress, STMPE811_REG_SYS_CTRL2, mode);	// Write the new register value


	I2C_writeByte(deviceAddress, STMPE811_REG_IO_AF, STMPE811_TOUCH_IO_ALL); // Select TSC pins in TSC alternate mode
	mode &= ~(STMPE811_TS_FCT | STMPE811_ADC_FCT);
	I2C_writeByte(deviceAddress, STMPE811_REG_SYS_CTRL2, mode);	// Write the new register value


	I2C_writeByte(deviceAddress, STMPE811_REG_ADC_CTRL1, 0x49);	// Select Sample Time, bit number and ADC Reference
	TIM_delay(2);

	I2C_writeByte(deviceAddress, STMPE811_REG_ADC_CTRL2, 0x01);	// Select the ADC clock speed: 3.25 MHz

	/* Select 2 nF filter capacitor */
	/* Configuration:
	- Touch average control    : 4 samples
	- Touch delay time         : 500 uS
	- Panel driver setting time: 500 uS
	*/
	I2C_writeByte(deviceAddress, STMPE811_REG_TSC_CFG, 0x9A);

	I2C_writeByte(deviceAddress, STMPE811_REG_FIFO_TH, 0x01); 	// Configure the Touch FIFO threshold: single point reading
	I2C_writeByte(deviceAddress, STMPE811_REG_FIFO_STA, 0x01);	// Clear the FIFO memory content
	I2C_writeByte(deviceAddress, STMPE811_REG_FIFO_STA, 0x00);	// Put the FIFO back into operation mode

	/* Set the range and accuracy pf the pressure measurement (Z) :
	- Fractional part :7a
	- Whole part      :1
	*/
	I2C_writeByte(deviceAddress, STMPE811_REG_TSC_FRACT_XYZ, 0x07);

	I2C_writeByte(deviceAddress, STMPE811_REG_TSC_I_DRIVE, 0x01); // Set the driving capability (limit) of the device for TSC pins: 50mA

	/* Touch screen control configuration (enable TSC):
	- No window tracking index
	- XYZ acquisition mode
	*/
	I2C_writeByte(deviceAddress, STMPE811_REG_TSC_CTRL, 0x01);

	I2C_writeByte(deviceAddress, STMPE811_REG_INT_STA, 0xFF);	// Clear all the status pending bits if any
	TIM_delay(5);
}



TouchDetect_t isTouched(void)
{

	uint8_t value = 0;

	value = I2C_readByte(deviceAddress, STMPE811_REG_TSC_CTRL);		// Read TSC Register

	if(value & STMPE811_TS_CTRL_STATUS)	// If touch
	{
		value = I2C_readByte(deviceAddress,STMPE811_REG_FIFO_SIZE);
		if(value > 0)
			return touched;
	}

	else
	{
		I2C_writeByte(deviceAddress, STMPE811_REG_FIFO_STA, 0x01); // Reset FIFO
		I2C_writeByte(deviceAddress, STMPE811_REG_FIFO_STA, 0x00); // Enable the FIFO again
	}
	return no_touch;
}


void getTouchValue(uint16_t *X, uint16_t *Y)
{
	uint32_t uldataXYZ;
	uint8_t  dataXYZ[4];
	uint16_t raw_X, raw_Y;

	// Calculate positions values
	I2C_readMultiByte(deviceAddress, STMPE811_REG_TSC_DATA_NON_INC, dataXYZ, 4) ;
	uldataXYZ = (dataXYZ[0] << 24)|(dataXYZ[1] << 16)|(dataXYZ[2] << 8)|(dataXYZ[3] << 0);

	raw_X = (uldataXYZ >> 20) & 0x00000FFF;
	raw_Y = (uldataXYZ >>  8) & 0x00000FFF;

	// Rearrange values into LCD dimensions
	if(raw_X < 250)
		raw_X = 250;
	if(raw_Y < 180)
		raw_Y = 180;

	raw_X -= 250;
	raw_Y -= 180;

	raw_X = round(raw_X/15);
	raw_Y = round(raw_Y/11);

	*X = 240 - raw_X;
	*Y = 320 - raw_Y;

	if(raw_X > 240)
		*X = 240;
	if(raw_Y > 320)
		*Y = 0;

	I2C_writeByte(deviceAddress, STMPE811_REG_FIFO_STA, 0x01); // Reset FIFO
	I2C_writeByte(deviceAddress, STMPE811_REG_FIFO_STA, 0x00); // Enable the FIFO again
}

