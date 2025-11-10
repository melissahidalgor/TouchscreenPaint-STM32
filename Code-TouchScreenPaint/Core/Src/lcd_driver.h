

#ifndef LCD_DRIVER_H_
#define LCD_DRIVER_H_


#include "ili9341.h"
#include "fonts.h"

#include "stm32f4xx.h"
#include "macros_utiles.h"
#include "GPIO.h"
#include "TIM.h"
#include "SPI_LCD.h"


#define LCD_BUF_LEN       (ILI9341_LCD_PIXEL_HEIGHT*ILI9341_LCD_PIXEL_WIDTH)
#define LCD_BUF_LEN_BYTES (LCD_BUF_LEN * 2)
#define BITS_IN_BYTE      (8)


void LCD_InitSerialInterface(void);
void LCD_CopyColorToFrameBuffer(uint16_t color);
void LCD_TransmitFrameBuffer(void);
void LCD_SetAddress (uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2);
void LCD_WriteChar  (uint8_t character, uint16_t backGroundColor, uint16_t charColor, uint16_t cursorX, uint16_t cursorY);

void LCD_InitGPIO(void);
void LCD_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
void LCD_writeString(char* msg, int xi, int yi);
void LCD_FillRect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);

#endif /* LCD_DRIVER_H_ */
