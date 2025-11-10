/*
 * GPIO_ConfigPin.h
 *
 *  Created on: 10 sept. 2024
 *      Author: Melissa
 */

#ifndef GPIO_H_
#define GPIO_H_

/* Includes */
#include "stm32f4xx.h"
#include "macros_utiles.h"

/* Useful constants */
#define NO 0

typedef enum MODE{
	INPUT  = 0,
	OUTPUT = 1,
	AF 	   = 2,
	ANALOG = 3
}MODE_TypeDef;

typedef enum OTYPER{
	PUSHP = 0,
	OPEND = 1,
}OTYPER_TypeDef;

typedef enum PUPD{
	NOPUPD = 0,
	PDOWN  = 1,
	PUP	   = 2
}PUPD_TypeDef;

typedef enum STATE{
	OFF = 0,
	ON	= 1
}STATE_TypeDef;

typedef enum SPEED{
	LOW     = 0,
	MEDIUM  = 1,
	HIGH	= 2,
	VHIGH   = 3,
}SPEED_TypeDef;

/* Function prototype */
void GPIO_configPIN(GPIO_TypeDef *port, uint8_t pin, MODE_TypeDef mode, OTYPER_TypeDef otyper, SPEED_TypeDef speed, PUPD_TypeDef pupd, uint8_t AFn);
void GPIO_writePIN(GPIO_TypeDef *port, uint8_t pin, STATE_TypeDef state);
uint32_t GPIO_readPIN(GPIO_TypeDef *port, uint8_t pin);

#endif /* GPIO_H_ */
