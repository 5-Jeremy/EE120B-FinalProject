#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "nokia5110.h"
#include "io.h"
#include "LCD_Custom.h"
#include "timer.h"

unsigned char GetNumDigits_us(unsigned short num) {
	if (num < 10) {
		return 1;
	}
	else if (num < 100) {
		return 2;
	}
	else if (num < 1000) {
		return 3;
	}
	else if (num < 10000) {
		return 4;
	}
	else {					// unsigned short cannot have more than 5 digits
		return 5;
	}
}

// Fills the array str with characters corresponding to the digits in num
void IntToString(unsigned short num, unsigned char *str) {
	unsigned char numDigits = GetNumDigits_us(num);
	unsigned char i;
	for (i = 0; i < numDigits; i++) {
		str[numDigits - (i + 1)] = (num % 10) + '0';
		num /= 10;
	}
}

void Nokia_WriteNum(unsigned short num) {
	unsigned char numDigits = GetNumDigits_us(num);
	unsigned char i;
	unsigned char str[numDigits];
	IntToString(num, str);
	for (i = 0; i < numDigits; i++) {
		nokia_lcd_write_char(str[i],1);
	}
}

int main(void)
{
     
	 DDRA = 0xFF; PORTA = 0x00;	// LCD Control Bus
	 DDRB = 0x7F; PORTB = 0x80;	// Nokia
	 DDRC = 0xFF; PORTC = 0x00;	// LCD Data Bus
	nokia_lcd_init();
	Custom_LCD_Init();
	nokia_lcd_clear();
	LCD_ClearScreen();
	unsigned short timeLeft = 0;		// 248 for 6 seconds, 340 for 8 seconds, 
	unsigned char press = 0;
	TimerSet(10);
	TimerOn();
	while(timeLeft < 600) {
		timeLeft++;
		nokia_lcd_clear();
		nokia_lcd_set_cursor(0,10);
		Nokia_WriteNum(timeLeft);
		nokia_lcd_render();
		while (!TimerFlag){	}
		TimerFlag = 0;
	}
	nokia_lcd_clear();
	nokia_lcd_set_cursor(0,10);
	Nokia_WriteNum(timeLeft);
	nokia_lcd_render();
    while (1) 
    {
		/*if (~PINB & 0x80 && !press) {
			press = 1;
		}
		else if (!press) {
			timeLeft++;
		}*/
		
		while (!TimerFlag){	}
		TimerFlag = 0;
    }
}

