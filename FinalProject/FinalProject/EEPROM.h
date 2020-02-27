#ifndef __EEPROM_h__
#define __EEPROM_h__

void EEPROM_StoreByte(unsigned short address, unsigned char data);
unsigned char EEPROM_ReadByte(unsigned short address);

#endif