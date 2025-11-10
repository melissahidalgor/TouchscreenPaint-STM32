/*
 * TIM.c
 *
 *  Created on: 28 nov. 2024
 *      Author: Melissa
 */

#include "TIM.h"

void TIM_config(uint64_t freq)
{
	const uint32_t CLK_Freq = 72000000/2; 	// CLK 72 MHz and prescaler APB1

	uint32_t PSC_value = 1;
	uint32_t ARR_value;

	RCC->APB1ENR |= BIT0;				// Enable TIM2 CLK

	TIM2->CR1  &= ~BIT0;					// Counter disable
	TIM2->ARR  &= ~0xFFFFFFFF;			// Clear ARR
	TIM2->PSC  &= ~0xFFFF;				// Clear PSC

	ARR_value = CLK_Freq/freq - 1;
	if(ARR_value > 0xFFFFFFFF)
	{
		while(ARR_value > 0xFFFFFFFF)
		{
			PSC_value *= 2;
			ARR_value = (ARR_value + 1)/PSC_value - 1;
		}
		TIM2->ARR |= ARR_value;
		TIM2->PSC |= PSC_value;
	}
	else
		TIM2->ARR |= ARR_value;

	TIM2->CR1 |= BIT0;						// Counter enable
}

void TIM_delay(uint64_t milis){
	for(int i=0; i<milis; i++){
		while(!(TIM2->SR & BIT0));				// Wait until counting ends
		TIM2->SR &= ~BIT0;													// Reset Flag
	}
}
