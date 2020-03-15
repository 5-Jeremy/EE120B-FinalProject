/*	Author: Jeremy Carleton jcarl023@ucr.edu
 *	Lab Section: 021
 *	Assignment: Custom Lab Project
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#ifndef __EEPROM_h__
#define __EEPROM_h__

void EEPROM_StoreByte(unsigned short address, unsigned char data);

unsigned char EEPROM_ReadByte(unsigned short address);

#endif