/*
 * keyboard.h
 *
 *  Created on: 28 nov. 2024
 *      Author: Melissa
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include "stm32f4xx.h"
#include "GPIO.h"
#include "TIM.h"


char keypad_tecla(void);			// Barrido del teclado
void keyboard_config(void);

#endif /* KEYBOARD_H_ */
