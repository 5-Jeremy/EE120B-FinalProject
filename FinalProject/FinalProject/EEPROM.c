#include <avr/io.h>
#include "EEPROM.h"
#include "io.h"

/*
EEAR	// Address reg (16-bit)
			// bits 9-0 store address
EEDR	// Data reg (8-bit)
EECR	// Control reg (8-bit)
			// bit 3: Ready Interrupt Enable
			// bit 2: Master Write Enable
			// bit 1: Write Enable
			// bit 0: Read Enable
*/

// Aliases for individual EECR bits
#define EEMWE EECR&0x04
#define EEWE EECR&0x02

// Use to set/clear bits on the EECR
#define SET_EEMWE() (EECR |= 0x04)
#define SET_EEWE() (EECR |= 0x02)
#define SET_EERE() (EECR |= 0x01)

void EEPROM_StoreByte(unsigned short address, unsigned char data) {
	while(EEWE) {}
	EEAR = address;
	EEDR = data;
	SET_EEMWE();
	SET_EEWE();
}

unsigned char EEPROM_ReadByte(unsigned short address) {
	while(EEWE) {}
	EEAR = address;
	SET_EERE();
	delay_ms(1);
	unsigned char data = EEDR;
	return data;
}