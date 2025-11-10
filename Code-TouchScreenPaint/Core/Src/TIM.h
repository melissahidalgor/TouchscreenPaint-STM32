/*
 * TIM.h
 *
 *  Created on: 28 nov. 2024
 *      Author: Melissa
 * TIMER 2
 */

#ifndef TIM_H_
#define TIM_H_

/* Includes */
#include "stm32f4xx.h"
#include "macros_utiles.h"

void TIM_config(uint64_t freq);
void TIM_delay(uint64_t milis);

#endif /* TIM_H_ */
