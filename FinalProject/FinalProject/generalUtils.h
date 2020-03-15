/*	Author: Jeremy Carleton jcarl023@ucr.edu
 *	Lab Section: 021
 *	Assignment: Custom Lab Project
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#ifndef GENERALUTILS_H_
#define GENERALUTILS_H_

unsigned char GetNumDigits_us(unsigned short num);

void IntToString(unsigned short num, unsigned char *str);

unsigned char GetNumDigits_ul(unsigned long num);

void IntToString_ul(unsigned long num, unsigned char *str);

#endif /* GENERALUTILS_H_ */