#ifndef LCDCTRL_H_
#define LCDCTRL_H_
// Contains the functions used by the LCDCtrl task

enum LC_states {LC_start, LC_noGame, LC_gameInProgress, LC_gameEnd};
int LC_Tick(int state) {
	static unsigned char i;
	switch(state) {
		case LC_start :
			state = LC_noGame;
			i = 0;
			break;
		case LC_noGame :
			state = LC_noGame;
			gameInProgress = 1;
			/*
			if (i >= 100) {
				unsigned short randNum = GenRand();
				LCD_Cursor(1);
				Custom_LCD_WriteNum(randNum);
				i = 0;
			}
			else {
				i++;
			}
			*/
			break;
		default: state = LC_start; break;
	}
	return state;
}
#endif /* LCDCTRL_H_ */