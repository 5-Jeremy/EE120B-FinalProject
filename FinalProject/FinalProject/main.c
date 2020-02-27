#include <avr/io.h>
#include "io.h"
#include "keypad.h"
// Custom headers
#include "LCD_Custom.h"
#include "EEPROM.h"

// Custom character codes
#define MEDAL_CODE 0
#define SQRT_CODE 1

int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    DDRC = 0xFF; PORTC = 0x00;
    DDRD = 0xFF; PORTD = 0x00;
    Custom_LCD_Init();
    LCD_ClearScreen();
	
    while (1) 
    {
		
    }
}


/*// Custom char test
	unsigned char i;
	for (i = 0; i <= 7; ++i) {
		LCD_Cursor(i+1);
		LCD_WriteData(i);
	}
	*/
	/* EEPROM test
	unsigned char readData;
	unsigned short addr = 0x0000;
	unsigned char writeData = 'a';
	//EEPROM_StoreByte(addr, writeData);
	readData = EEPROM_ReadByte(addr);
	LCD_WriteData(readData);
	*/