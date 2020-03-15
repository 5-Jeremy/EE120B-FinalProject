/*	Author: Jeremy Carleton jcarl023@ucr.edu
 *	Lab Section: 021
 *	Assignment: Custom Lab Project
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#ifndef RANDOM_H_
#define RANDOM_H_

unsigned short GenNewSeed(unsigned short delay);

unsigned long long GenRand();

unsigned char GenRandWithinRange (unsigned char min, unsigned char max);

#endif /* RANDOM_H_ */