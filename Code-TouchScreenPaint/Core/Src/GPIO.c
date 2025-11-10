/*
 * GPIO_ConfigPin.c
 *
 *  Created on: 10 sept. 2024
 *      Author: Melissa
 */

#include "GPIO.h"

static uint32_t BIT[32]={BIT0, BIT1, BIT2, BIT3, BIT4, BIT5, BIT6, BIT7, BIT8, BIT9, BIT10, BIT11, BIT12, BIT13, BIT14, BIT15, BIT16, BIT17, BIT18, BIT19, BIT20, BIT21, BIT22, BIT23, BIT24, BIT25, BIT26, BIT27, BIT28, BIT29, BIT30, BIT31};


void GPIO_configPIN(GPIO_TypeDef *port, uint8_t pin, MODE_TypeDef mode, OTYPER_TypeDef otyper, SPEED_TypeDef speed, PUPD_TypeDef pupd, uint8_t AFn)
{
	// RCC Config
	if(GPIOA == port)
		RCC->AHB1ENR |= BIT0;
	else if(GPIOB == port)
		RCC->AHB1ENR |= BIT1;
	else if(GPIOC == port)
		RCC->AHB1ENR |= BIT2;
	else if(GPIOD == port)
		RCC->AHB1ENR |= BIT3;
	else if(GPIOE == port)
		RCC->AHB1ENR |= BIT4;
	else if(GPIOF == port)
		RCC->AHB1ENR |= BIT5;
	else if(GPIOG == port)
		RCC->AHB1ENR |= BIT6;
	else if(GPIOH == port)
		RCC->AHB1ENR |= BIT7;
	else if(GPIOI == port)
		RCC->AHB1ENR |= BIT8;

	// MODE Config
	switch(mode)
	{
		case(INPUT) :
			port->MODER &= ~(BIT[(2*pin + 1)] | BIT[(2*pin)]); 	 // INPUT (00)
			break;
		case(OUTPUT) :
			port->MODER &= ~BIT[(2*pin + 1)];					 // OUT (01)
			port->MODER |= BIT[(2*pin)];
			break;
		case(AF) :
			port->MODER |= BIT[(2*pin + 1)];					 // ALTERNATE FUNCTION (10)
			port->MODER &= ~BIT[(2*pin)];
			break;
		case(ANALOG) :
			port->MODER |= (BIT[(2*pin + 1)] | BIT[(2*pin)]);  	 // ANALOG (11)
			break;
		default:												 //Leave reset values
			break;
	}

	if(OPEND == otyper)
	{
	 port -> OTYPER |= BIT[pin]; 	 // Open Drain (1)
	} // Default Push Pull

	// SPEED Config
	switch(speed)
	{
		case(LOW) :
			port->OSPEEDR &= ~(BIT[(2*pin + 1)] | BIT[(2*pin)]); 	 // LOW (00)
			break;
		case(MEDIUM) :
			port->OSPEEDR &= ~BIT[(2*pin + 1)];					 	// MEDIUM (01)
			port->OSPEEDR |= BIT[(2*pin)];
			break;
		case(HIGH) :
			port->OSPEEDR |= BIT[(2*pin + 1)];					 	// HIGH (10)
			port->OSPEEDR &= ~BIT[(2*pin)];
			break;
		case(VHIGH) :
			port->OSPEEDR |= (BIT[(2*pin + 1)] | BIT[(2*pin)]);  	 // VERY HIGH (11)
			break;
		default:												 //Leave reset values
			break;
	}

	// PULL-UP, PULL-DOWN Config
	if(PUP == pupd){
		port->PUPDR &= ~BIT[(2*pin + 1)];						// PULL-UP (01)
		port->PUPDR |= BIT[(2*pin)];
	}else{
		if(PDOWN == pupd)
		{
			port->PUPDR |= BIT[(2*pin) + 1];					// PULL-DOWN (10)
			port->PUPDR &= ~BIT[(2*pin)];
		}
	}

	// Alternate Function Config
	if(AF == mode)
	{
		uint8_t indexAF;
		uint8_t indexPin;
		if(pin > 7)					// Lower or Upper Register
		{
			indexAF = 1;
			indexPin = pin-8;
		}
		else
		{
			indexAF = 0;
			indexPin = pin;
		}
		for (int i = 0; i < 4; i++)
		{
			if ((AFn & (1<<i)) != 0)	 // Atomic AND
			{
				port->AFR[indexAF] |= BIT[4*indexPin + i];
			}
		}//for
	}//if

}//function


void GPIO_writePIN(GPIO_TypeDef *port, uint8_t pin, uint8_t state)
{
	if(ON == state)
		port->ODR |= BIT[pin];
	else
		port->ODR &= ~BIT[pin];
}

uint32_t GPIO_readPIN(GPIO_TypeDef *port, uint8_t pin){
	uint32_t state;

	state = port->IDR & BIT[pin];
	if(state != 0)
		state = 1;
	return state;
}


