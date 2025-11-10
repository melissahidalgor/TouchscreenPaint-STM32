/*
 * STMEP811.h
 *
 *  Created on: 27 nov. 2024
 *      Author: Melissa
 *      Based on the codes of https://blog.embeddedexpert.io and https://stm32f4-discovery.net
 */

#ifndef STMEP811_H_
#define STMEP811_H_

#include "STM32F4xx.h"
#include "I2C_LCDT.h"
#include "math.h"
#include "TIM.h"

#define deviceAddress 0x41


typedef enum TouchDetect
{
	touched =1,
	no_touch=0
}TouchDetect_t;

// Registers Addresses
#define STMPE811_REG_SYS_CTRL1 			0x03  // Reset control
#define STMPE811_REG_SYS_CTRL2 			0x04  // Clock control
#define STMPE811_REG_IO_AF              0x17  // Alternate function register
#define STMPE811_REG_INT_STA            0x0B  // Interrupt status register

#define STMPE811_REG_ADC_CTRL1          0x20  // ADC control 1
#define STMPE811_REG_ADC_CTRL2          0x21  // ADC control 2

#define STMPE811_REG_FIFO_TH            0x4A  // FIFO threshold
#define STMPE811_REG_FIFO_STA           0x4B  // FIFO status
#define STMPE811_REG_FIFO_SIZE          0x4C  // FIFO size

#define STMPE811_REG_TSC_CFG            0x41  // Touch Configuration
#define STMPE811_REG_TSC_FRACT_XYZ      0x56  // Touchscreen controller FRACTION_Z
#define STMPE811_REG_TSC_I_DRIVE        0x58  // Touchscreen controller drive
#define STMPE811_REG_TSC_CTRL           0x40  // Touchscreen controller control register
#define STMPE811_REG_TSC_DATA_NON_INC   0xD7  // Touchscreen controller DATA Non-Incremental
#define STMPE811_TS_CTRL_STATUS         0x80  // Touchscreen status

// IO expander facilities
#define STMPE811_ADC_FCT                0x01
#define STMPE811_TS_FCT                 0x02
#define STMPE811_IO_FCT                 0x04

// Touch Screen Pins definition
//#define STMPE811_TOUCH_YD               0x80  // PIN7
//#define STMPE811_TOUCH_XD               0x40  // PIN6
//#define STMPE811_TOUCH_YU               0x20  // PIN5
//#define STMPE811_TOUCH_XU               0x10  // PIN4
#define STMPE811_TOUCH_IO_ALL           0x00



void STMPE811_Touch_Enable(void);
TouchDetect_t isTouched(void);
void getTouchValue(uint16_t *X, uint16_t *Y);

#endif /* STMEP811_H_ */
