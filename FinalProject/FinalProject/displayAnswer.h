#ifndef DISPLAYANSWER_H_
#define DISPLAYANSWER_H_
// Contains the functions used by the displayAnswer task

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
				i = ++i % 20;
				unsigned char nd = isInputAvailable ? GetNumDigits_us(currAnswer) : 0;
				unsigned char arrowLoc = 32-nd;
				LCD_Cursor(arrowLoc);
				if (i <= 10) LCD_WriteData(2);	// Controls arrow blinking
				else LCD_WriteData(' ');
				if (isInputAvailable) {		
					if ((currAnswer != lastAnswer) || (currAnswer == 0)) {	// Only re-print the answer if it has been changed
						Custom_LCD_WriteNum(currAnswer);
					}
				}
				/*
				if (isInputAvailable && ((currAnswer != lastAnswer) || (currAnswer == 0))) {
					unsigned char nd = GetNumDigits_us(currAnswer);
					LCD_Cursor(32-(nd));
					if (i <= 10) LCD_WriteData(2);	// Controls arrow blinking
					else LCD_WriteData(' ');
					Custom_LCD_WriteNum(currAnswer);
				}
				else {
					LCD_Cursor(32);
					
					if (lastAnswer > 0) {	// If true, the player has just cleared their input,
						LCD_Cursor(27);		// so it must be erased
						unsigned char j;
						for (j = 0; j < 6; j++) {
							LCD_WriteData(' ');
						}
					}
					
					if (i <= 10) LCD_WriteData(2);	// Controls arrow blinking
					else LCD_WriteData(' ');
				}
			}
			*/
				lastAnswer = currAnswer;
			}
			break;
		default: state = DA_start; break;
	}
	return state;
}

#endif /* DISPLAYANSWER_H_ */