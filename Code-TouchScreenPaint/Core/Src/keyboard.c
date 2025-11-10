/*
 * keyboard.c
 *
 *  Created on: 28 nov. 2024
 *      Author: Melissa
 */

#include "keyboard.h"
#include "cmsis_os.h"

void keyboard_config(void)
{
	// Rows as inputs with pull-up
	GPIO_configPIN(GPIOD, 7, INPUT, PUSHP, LOW, PUP, NO);
	GPIO_configPIN(GPIOD, 6, INPUT, PUSHP, LOW, PUP, NO);
	GPIO_configPIN(GPIOD, 5, INPUT, PUSHP, LOW, PUP, NO);
	GPIO_configPIN(GPIOD, 4, INPUT, PUSHP, LOW, PUP, NO);

	// Columns as outputs
	GPIO_configPIN(GPIOD, 3, OUTPUT, PUSHP, LOW, NOPUPD, NO);
	GPIO_configPIN(GPIOD, 2, OUTPUT, PUSHP, LOW, NOPUPD, NO);
	GPIO_configPIN(GPIOD, 1, OUTPUT, PUSHP, LOW, NOPUPD, NO);
	GPIO_configPIN(GPIOD, 0, OUTPUT, PUSHP, LOW, NOPUPD, NO);
}

char keypad_tecla(void)
{
	uint32_t col[4] = {0xFE, 0xFD, 0xFB, 0xF7};		// Column activation values - OUT
	uint32_t fil[4] = {0x10, 0x20, 0x40, 0x80};		// Row scanning values - IN

	char key_map[4][4] = {
	    {'1', '2', '3', '+'},
	    {'4', '5', '6', '-'},
	    {'7', '8', '9', 'A'},
	    {'D', '0', 'C', 'B'}
	};

	for(int i = 0; i < 4; i++)	  	// Sweep Columns
	{
		GPIOD -> ODR = col[i];    	// Activate each column

		for(int j = 0; j < 4; j++)	// Sweep Rows
		{
			if((GPIOD -> IDR & fil[j]) == 0 ) // If key pressed in current row
			{
				osDelay(10);	// Debounce delay
				return key_map[j][i];	// Return key value
			}
		} // for j
	} // for i
	return '\0';						 // Return control value, no key pressed
}

