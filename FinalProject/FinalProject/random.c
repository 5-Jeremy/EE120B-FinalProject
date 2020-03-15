/*	Author: Jeremy Carleton jcarl023@ucr.edu
 *	Lab Section: 021
 *	Assignment: Custom Lab Project
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
// Seed variable used by this file only
unsigned short seed;

// Parameters for Linear Congruential Generator
unsigned long multiplier = 725064787;
unsigned long increment = 851897918;
unsigned long modulus = 247814423;


void GenNewSeed(unsigned short delay) {
	seed = (delay^3) % 500;		// Convert recorded delay to a seed
}

unsigned long long GenRand() {
	seed = (multiplier * seed + increment) % modulus;
	return seed;
}

unsigned char GenRandWithinRange (unsigned char min, unsigned char max) {
	if (min > max) return 0; // Invalid arguments
	unsigned long long num = GenRand();
	unsigned char range = max - min + 1;
	return (num % range) + min;
}