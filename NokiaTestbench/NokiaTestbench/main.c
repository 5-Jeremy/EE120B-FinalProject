#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "nokia5110.h"
#include "io.h"
#include "LCD_Custom.h"

int main(void)
{
     
	 DDRA = 0xFF; PORTA = 0x00;	// LCD Control Bus
	 DDRB = 0xFF; PORTB = 0x00;	// Nokia
	 DDRC = 0xFF; PORTC = 0x00;	// LCD Data Bus
	 /*
	 nokia_lcd_init();
     nokia_lcd_clear();
	 nokia_lcd_set_cursor(0,0);
     nokia_lcd_write_string(,1);
	 nokia_lcd_render();
	 */
	 Custom_LCD_Init();
	 LCD_ClearScreen();
	 LCD_Cursor(1);
	 LCD_WriteData(0);
	 LCD_WriteData(1);
	 LCD_WriteData(2);
    while (1) 
    {
    }
}

