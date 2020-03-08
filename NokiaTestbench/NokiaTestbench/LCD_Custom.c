#include <avr/io.h>
#include "io.h"
#include "LCD_Custom.h"

// Byte sequence for custom char 0 (medallion)
unsigned char custom_0[] = {0x1F, 0x0E, 0x04, 0x04, 0x0E, 0x1F, 0x0E, 0x04,};
// Byte sequence for custom char 1 (square root)
unsigned char custom_1[] = {0x03, 0x04, 0x04, 0x04, 0x04, 0x04, 0x14, 0x08,};
// Byte sequence for custom char 2 (bold arrow)
unsigned char custom_2[] = {0x00, 0x0C, 0x0E, 0x0F, 0x0F, 0x0E, 0x0C, 0x00,};
// Byte sequence for custom char 3 (trophy)
unsigned char custom_3[] = {0x00, 0x1F, 0x0E, 0x0E, 0x04, 0x04, 0x0E, 0x1F,};
// Unused
unsigned char custom_4[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,};
unsigned char custom_5[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,};
unsigned char custom_6[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,};
unsigned char custom_7[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,};

// Package data into an array
unsigned char* customCharData[8] = {custom_0,custom_1,custom_2,custom_3,custom_4,custom_5,custom_6,custom_7,};
	
/* Performs normal initialization of LCD display, and also writes custom character data to the display's memory */
void Custom_LCD_Init(void) {
	delay_ms(100);
	LCD_WriteCommand(0x38);
	LCD_WriteCommand(0x06);
	LCD_WriteCommand(0x0e);
	LCD_WriteCommand(0x01);
	delay_ms(10);
	// Load custom character data
	unsigned char i,j;
	unsigned char CGRAM_addr = 0x40;
	//for (i = 0; i < sizeof(customCharData)/8; i++) {
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			LCD_WriteCommand(CGRAM_addr++);
			LCD_WriteData(customCharData[i][j]);
			delay_ms(1);
		}
	}
	delay_ms(10);
}

/* Prints the number num to the LCD display, starting at the current cursor location */
void Custom_LCD_WriteNum(unsigned short num) {
	unsigned char numDigits = GetNumDigits_us(num);
	unsigned char i;
	unsigned char str[numDigits];
	IntToString(num, str);
	/*
	for (i = 0; i < numDigits; i++) {
		str[numDigits - (i + 1)] = (num % 10) + '0';
		num /= 10;
	}
	*/
	for (i = 0; i < numDigits; i++) {
		LCD_WriteData(str[i]);
	}
}