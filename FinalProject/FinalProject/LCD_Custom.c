#include <avr/io.h>
#include "io.h"
#include "LCD_Custom.h"

// Byte sequence for custom char 0 (medallion)
unsigned char custom_0[] = {0x1F, 0x0E, 0x04, 0x04, 0x0E, 0x1F, 0x0E, 0x04,};
// Byte sequence for custom char 1 (square root)
unsigned char custom_1[] = {0x03, 0x04, 0x04, 0x04, 0x04, 0x04, 0x14, 0x08,};
	
unsigned char custom_2[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,};
unsigned char custom_3[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,};
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
	LCD_WriteCommand(0x0f);
	LCD_WriteCommand(0x01);
	delay_ms(10);
	// Load custom character data
	LCD_WriteCommand(0x40);
	unsigned char i,j;
	for (i = 0; i < sizeof(customCharData)/8; i++) {
		for (j = 0; j < 8; j++) {
			LCD_WriteData(customCharData[i][j]);
		}
	}
	
	/*
	unsigned char i,j;
	for (i = 0; i < 2; i++) {
		//LCD_WriteCommand(0x40 + 8*i);
		for (j = 0; j < 8; j++) {
			LCD_WriteData(customCharData[i][j]);
		}
	}
	*/
	delay_ms(10);
}