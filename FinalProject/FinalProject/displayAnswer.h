/*	Author: Jeremy Carleton jcarl023@ucr.edu
 *	Lab Section: 021
 *	Assignment: Custom Lab Project
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#ifndef DISPLAYANSWER_H_
#define DISPLAYANSWER_H_
// Contains the functions used by the displayAnswer task

#define ARROW_CODE 2

enum DA_states {DA_start, DA_main};
int DA_Tick(int state) {
	static unsigned long lastAnswer;	// Used to track changes in user input
	static unsigned char i;
	switch(state) {
		case DA_start :
			state = DA_main;
			lastAnswer = 0;
			i = 0;
			break;
		case DA_main : 
			if(gameInProgress) {
				i = ++i % 50;
				unsigned char nd = isInputAvailable ? GetNumDigits_us(currAnswer) : 0;
				unsigned char arrowLoc = 32-nd;
				LCD_Cursor(arrowLoc);
				if (i <= 25) LCD_WriteData(ARROW_CODE);	// Controls arrow blinking
				else LCD_WriteData(' ');
				if (isInputAvailable) {		
					if ((currAnswer != lastAnswer) || (currAnswer == 0)) {	// Only re-print the answer if it has been changed
						Custom_LCD_WriteNum(currAnswer);
					}
				}
				lastAnswer = currAnswer;
			}
			break;
		default: state = DA_start; break;
	}
	return state;
}

#endif /* DISPLAYANSWER_H_ */