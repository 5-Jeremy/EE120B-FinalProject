#ifndef MENUCTRL_H_
#define MENUCTRL_H_
// Contains the functions used by the menuControl task

void WriteScreen();

enum MC_states {MC_start, MC_main, MC_prepareGame,  MC_gameInProgress, MC_gameEnd};
enum screens {screen_mainMenu, screen_startGame, screen_inGame, screen_gameEnd, screen_difficultySelect, screen_settings, screen_viewScores} screen;
int MC_Tick(int state) {
	switch (state) {
		case MC_start : 
			state = MC_main;
			newGameSelected = 0;
			gameInProgress = 0;
			difficulty = EASY;
			buzzer = 1;
			screen = screen_mainMenu;
			break;
		case MC_main :
			newGameSelected = 0;
			gameInProgress = 0;
			if (!QueueIsEmpty(menuQ)) {
				if (QueueDequeue(menuQ) == 'A') {
					state = MC_prepareGame;
					newGameSelected = 1;
					screen = screen_startGame;
				}
				else {
					state = MC_main;
				}
			}
			break;
		case MC_prepareGame :
			if (lastPressed == '#' && keyDown) {
				state = MC_gameInProgress;
				newGameSelected = 0;
				gameInProgress = 1;
			}
			else {
				state = MC_prepareGame;
			}
			break;
		case MC_gameInProgress :
			if (lastPressed == '*' && keyDown) {
				state = MC_gameEnd;
				gameInProgress = 0;
				
			}
			else {
				state = MC_gameInProgress;
			}
			break;
		case MC_gameEnd :
			if (lastPressed == '#' && keyDown) {
				state = MC_main;
			}
			else {
				state = MC_gameEnd;
			}
			break;
		default: state = MC_start; break;
	}
	WriteScreen();
	return state;
}

void PrintDifficultyIdentifier() {
	switch(difficulty) {
		case EASY : nokia_lcd_write_char(0x45,1); break;
		case MEDIUM : nokia_lcd_write_char(0x4D,1); break;
		case HARD : nokia_lcd_write_char(0x48,1); break;
	}
}

void WriteScreen() {
	nokia_lcd_clear();
	switch(screen) {
		case screen_mainMenu :
			 nokia_lcd_set_cursor(0,0);
			 nokia_lcd_write_string("MAIN MENU",1);
			 nokia_lcd_set_cursor(0,10);
			 nokia_lcd_write_string("A: New game",1);
			 nokia_lcd_set_cursor(0,20);
			 nokia_lcd_write_string("B: Difficulty",1);
			 nokia_lcd_set_cursor(0,30);
			 nokia_lcd_write_string("C: Highscores",1);
			 nokia_lcd_set_cursor(0,40);
			 nokia_lcd_write_string("D: Settings",1);
			 break;
		case screen_startGame :
			nokia_lcd_set_cursor(0,0);
			nokia_lcd_write_string("NEW GAME",1);
			nokia_lcd_set_cursor(0,10);
			nokia_lcd_write_string("Difficulty: ",1);
			PrintDifficultyIdentifier();
			nokia_lcd_set_cursor(0,30);
			nokia_lcd_write_string("Press # key to",1);
			nokia_lcd_set_cursor(0,40);
			nokia_lcd_write_string("begin",1);
			break;
		default: break;
	}
	nokia_lcd_render();
}


#endif /* MENUCTRL_H_ */