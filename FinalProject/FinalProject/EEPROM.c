/*	Author: Jeremy Carleton jcarl023@ucr.edu
 *	Lab Section: 021
 *	Assignment: Custom Lab Project
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
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

/* Writes the data passed in the second argument to the EEPROM address passed in the first argument */
void EEPROM_StoreByte(unsigned short address, unsigned char data) {
	while(EEWE) {}		// Do not attempt to write data while a write task is still in progress
	EEAR = address;
	EEDR = data;
	SET_EEMWE();		// Master write-enable
	SET_EEWE();			// Write-enable
}

/* Reads the data in the EEPROM address passed in the argument and returns that byte */
unsigned char EEPROM_ReadByte(unsigned short address) {
	while(EEWE) {}		// Do not attempt to read data while a write task is still in progress
	EEAR = address;
	SET_EERE();
	delay_ms(1);
	unsigned char data = EEDR;	// Read from data register after waiting 1ms for data to be fetched
	return data;
}