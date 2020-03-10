#ifndef MENUCTRL_H_
#define MENUCTRL_H_
// Contains the functions used by the menuControl task

void WriteScreen();

enum MC_states {MC_start, MC_main, MC_prepareGame,  MC_gameInProgress, MC_gameEnd, MC_viewScores, MC_wipe, MC_setDifficulty};
enum screens {screen_mainMenu, screen_startGame, screen_inGame, screen_gameEnd, screen_difficultySelect, screen_viewScores};
int MC_Tick(int state) {
	static int screen;
	static int lastScreen;
	static unsigned char delayTimer;
	unsigned char newInput;
	lastScreen = screen;
	switch (state) {
		case MC_start : 
			state = MC_main;
			newGameSelected = 0;
			gameInProgress = 0;
			difficulty = EASY;
			scoreReadNum = -1;
			wipeScores = 0;
			returnToMenu = 0;
			screen = screen_mainMenu;
			WriteScreen(screen);
			break;
		case MC_main :
			newGameSelected = 0;
			gameInProgress = 0;
			returnToMenu = 0;
			if (!QueueIsEmpty(menuQ)) {
				newInput = QueueDequeue(menuQ);
				if (newInput == 'A') {
					state = MC_prepareGame;
					newGameSelected = 1;
					screen = screen_startGame;
				}
				else if (newInput == 'B') {
					state = MC_setDifficulty;
					screen = screen_difficultySelect;
				}
				else if (newInput == 'C') {
					state = MC_viewScores;
					scoreReadNum = 1;
					screen = screen_viewScores;
				}
				else if (newInput == 'D') {
					state = MC_wipe;
					wipeScores = 1;
				}
				else {
					state = MC_main;
				}
			}
			else {
				state = MC_main;
			}
			break;
		case MC_prepareGame :
			if (!QueueIsEmpty(menuQ)) {
				if (QueueDequeue(menuQ) == '#') {
					state = MC_gameInProgress;
					newGameSelected = 0;
					gameInProgress = 1;
					screen = screen_inGame;
					WriteScreen(screen);
				}
				else {
					state = MC_prepareGame;
				}
			}
			break;
		case MC_gameInProgress :
			state = MC_gameInProgress;
			if (gameFin) {
				state = MC_gameEnd;
				gameInProgress = 0;
				delayTimer = 0;
				screen = screen_gameEnd;
			}
			if (screenUpdate) WriteScreen(screen);	// Update the screen to reflect change in lives/score/question #
			break;
		case MC_gameEnd :
			WriteScreen(screen);
			if (!QueueIsEmpty(menuQ) && delayTimer >= 20) {
				if (QueueDequeue(menuQ) == 'A') {
					state = MC_main;
					returnToMenu = 1;
					screen = screen_mainMenu;
				}
			}
			else {
				state = MC_gameEnd;
				if (delayTimer < 20) delayTimer++;
			}
			break;
		case MC_viewScores :
			state = MC_viewScores;
			if (!QueueIsEmpty(menuQ)) {
				newInput = QueueDequeue(menuQ);
				if (newInput == 'A') {
					scoreReadNum = 1;
				}
				else if (newInput == 'B') {
					if (scoreReadNum < 10) {
						scoreReadNum++;
					}
				}
				else if (newInput == 'C') {
					if (scoreReadNum > 1) {
						scoreReadNum--;
					}
				}
				else if (newInput == 'D') {
					state = MC_main;
					scoreReadNum = 0;
					screen = screen_mainMenu;
				}
				else {
					scoreReadNum = 0;
				}
			}
			else {
				state = MC_viewScores;
			}
			break;
		case MC_wipe :
			state = MC_main;
			wipeScores = 0;
			break;
		case MC_setDifficulty :
			state = MC_setDifficulty;
			if (!QueueIsEmpty(menuQ)) {
				newInput = QueueDequeue(menuQ);
				if (newInput == 'A') {
					state = MC_main;
					screen = screen_mainMenu;
					difficulty = EASY;
				}
				else if (newInput == 'B') {
					state = MC_main;
					screen = screen_mainMenu;
					difficulty = MEDIUM;
				}
				else if (newInput == 'C') {
					state = MC_main;
					screen = screen_mainMenu;
					difficulty = HARD;
				}
			}
			break;
		default: state = MC_start; break;
	}
	inScoreMenu = screen == screen_viewScores;
	if (screen != lastScreen && state != MC_gameInProgress) WriteScreen(screen);	// Only update the screen if needed
	return state;
}

void PrintDifficultyIdentifier() {
	switch(difficulty) {
		case EASY : nokia_lcd_write_char(0x45,1); break;
		case MEDIUM : nokia_lcd_write_char(0x4D,1); break;
		case HARD : nokia_lcd_write_char(0x48,1); break;
	}
}

void Nokia_WriteNum(unsigned short num) {
	unsigned char numDigits = GetNumDigits_us(num);
	unsigned char i;
	unsigned char str[numDigits];
	IntToString(num, str);
	for (i = 0; i < numDigits; i++) {
		nokia_lcd_write_char(str[i],1);
	}
}

void WriteScreen(int screen) {
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
			 nokia_lcd_write_string("D: Wipe scores",1);
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
		case screen_inGame :
			nokia_lcd_set_cursor(0,0);				// Current question number
			nokia_lcd_write_string("Question ",1);
			Nokia_WriteNum(problemsComplete + 1);
			nokia_lcd_write_string("/",1);
			Nokia_WriteNum(5);
			//Nokia_WriteNum(15);			//*********************************** Need to fix
			nokia_lcd_set_cursor(0,10);				// Current score
			nokia_lcd_write_string("Score: ",1);
			Nokia_WriteNum(score);
			nokia_lcd_set_cursor(0,20);
			nokia_lcd_write_string("Lives: ",1);	// Lives remaining
			Nokia_WriteNum(numLives);
			break;
		case screen_gameEnd :
			nokia_lcd_set_cursor(0,0);
			nokia_lcd_write_string("GAME END",1);
			nokia_lcd_set_cursor(0,10);
			nokia_lcd_write_string("Final Score:",1);
			nokia_lcd_set_cursor(0,20);
			Nokia_WriteNum(score);
			nokia_lcd_set_cursor(0,30);
			if (gameLost) {									// Tell user whether the score for this game was saved
				nokia_lcd_write_string("(Not saved)",1);	
			}
			else if (scoreWritten == 0) {					// Do not display anything here until scoreWritten is updated
				nokia_lcd_write_string("(Not saved)",1);	// to indicate whether the score was saved
			}
			else if ((scoreWritten > 0) && (scoreWritten <= 10)) {
				nokia_lcd_write_string("(Saved to #",1);
				Nokia_WriteNum(scoreWritten);
				nokia_lcd_write_char(')',1);
			}
			nokia_lcd_set_cursor(0,40);
			nokia_lcd_write_string("Press A",1);
			break;
		case screen_viewScores :
			nokia_lcd_set_cursor(0,0);
			nokia_lcd_write_string("HIGHSCORES",1);
			nokia_lcd_set_cursor(0,10);
			nokia_lcd_write_string("A: Jump to #1",1);
			nokia_lcd_set_cursor(0,20);
			nokia_lcd_write_string("B: Next",1);
			nokia_lcd_set_cursor(0,30);
			nokia_lcd_write_string("C: Previous",1);
			nokia_lcd_set_cursor(0,40);
			nokia_lcd_write_string("D: Return",1);
			break;
		case screen_difficultySelect :
			nokia_lcd_set_cursor(0,0);
			nokia_lcd_write_string("DIFFICULTY",1);
			nokia_lcd_set_cursor(0,10);
			nokia_lcd_write_string("Current: ",1);
			PrintDifficultyIdentifier();
			nokia_lcd_set_cursor(0,20);
			nokia_lcd_write_string("A: EASY",1);
			nokia_lcd_set_cursor(0,30);
			nokia_lcd_write_string("B: MEDIUM",1);
			nokia_lcd_set_cursor(0,40);
			nokia_lcd_write_string("C: HARD",1);
			break;
		default: break;
	}
	nokia_lcd_render();
}


#endif /* MENUCTRL_H_ */