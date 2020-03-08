#ifndef GAMEMASTER_H_
#define GAMEMASTER_H_
// Contains the functions used by the gameMaster task

// Number of lives the player is given at the start of a new game
#define START_LIVES 3
// Largest standalone number that can be used in a problem in easy mode
#define MAX_RAND_NUM_EZ 9
// Largest standalone number that can be used in a problem in medium mode
#define MAX_RAND_NUM_MED 20
// Largest standalone number that can be used in a problem in hard mode
#define MAX_RAND_NUM_HRD 50
// Time given per question in easy mode (seconds)
#define TIME_GIVEN_EZ 6
// Time given per question in medium mode (seconds)
#define TIME_GIVEN_MED 8
// Time given per question in hard mode (seconds)
#define TIME_GIVEN_HRD 10
// Greatest value permitted for a question's answer
//#define MAX_ANSWER_VAL 99999
#define MAX_ANSWER_VAL 9999

unsigned short GenerateQuestion();
unsigned char IsDigit(unsigned char data);
unsigned char Char2Digit(unsigned char data);

enum GM_states {GM_start, GM_wait, GM_newQuestion, GM_waitForAnswer, GM_timeOut, GM_correct, GM_livesOut, GM_win, GM_end};
int GM_Tick(int state) {
	static unsigned short solution;
	switch(state) {
		case GM_start :
			state = GM_wait;
			score = 0;
			timeLeft = 0;
			currAnswer = 0;
			isInputAvailable = 0;
			solution = 0;
			break;
		case GM_wait :
			state = gameInProgress ? GM_newQuestion : GM_wait;
			if (state == GM_newQuestion) {
				score = 0;
				switch(difficulty) {
					case EASY : timeLeft = TIME_GIVEN_EZ; break;
					case MEDIUM : timeLeft = TIME_GIVEN_MED; break;
					case HARD : timeLeft = TIME_GIVEN_HRD; break;
					default : timeLeft = TIME_GIVEN_EZ; break;
				}
			}
			break;
		case GM_newQuestion :
			state = GM_waitForAnswer;
			solution = GenerateQuestion();
			//LCD_Cursor(17);
			//Custom_LCD_WriteNum(solution);
			isInputAvailable = 0;
			currAnswer = 0;
			break;
		case GM_waitForAnswer :
			state = gameInProgress ? GM_waitForAnswer : GM_wait;
			if (!QueueIsEmpty(gameQ)) {
				unsigned char newInput = QueueDequeue(gameQ);
				if (newInput == '*') {
					currAnswer = 0;
					isInputAvailable = 0;
					LCD_Cursor(28);		// Erase existing input from the display
					unsigned char j;
					for (j = 0; j < 6; j++) {
						LCD_WriteData(' ');
					}
				}
				else if (newInput == '#') {
					
				}
				else if (IsDigit(newInput)) {
					if (currAnswer <= MAX_ANSWER_VAL/10) {	// Only accept more digits if they will not exceed the limit
						currAnswer = currAnswer * 10;
						currAnswer += Char2Digit(newInput);
					} 
					if (!isInputAvailable) isInputAvailable = 1;	// There must be input to write now
				}
			}
			if (state == GM_wait) {
				LCD_ClearScreen();
			}
			break;
		default: state = GM_start; break;
	}
	return state;
}
	
/* The following functions are used to generate random mathematical expressions to be solved */

enum Q_types {Add_2, Add_3, Sub_2, Sub_3, Mixed_3, Mult_2, Div_2,	// Easy questions (0-6)
			Mult_3, Add_2_Div, Add_2_Mult, Simple_exp,				// Medium questions (7-10)
			Simple_factorial};										// Hard questions
#define EZ_Q_MAX_NUM 6		// Define the values that correspond to the upper bound of the value that
#define MED_Q_MAX_NUM 10	// can be used to select a type based on the difficulty
#define HRD_Q_MAX_NUM 11
// Randomly selects a question type from the pool of available questions, which varies based on difficulty
int GetQuestionType() {
	int num;
	switch(difficulty) {
		case EASY : num = GenRand() % EZ_Q_MAX_NUM + 1; break;
		case MEDIUM : num = GenRand() % MED_Q_MAX_NUM + 1; break;
		case HARD : num = GenRand() % HRD_Q_MAX_NUM + 1; break;
		default: num = GenRand() % EZ_Q_MAX_NUM + 1; break;			// Default to easy
	}
	return num;
}

//Displays a randomly generated question on the top row of the LCD screen and returns the correct answer
unsigned short GenerateQuestion() {
	//int type = GetQuestionType();
	int type = GenRand() % 3;
	unsigned char randNumLimit;
	unsigned char n1,n2,n3;
	switch(difficulty) {
		case EASY : randNumLimit = MAX_RAND_NUM_EZ + 1; break;		// Set the limit for the magnitude of a 
		case MEDIUM : randNumLimit = MAX_RAND_NUM_MED + 1; break;	// randomly generated number to be used in
		case HARD : randNumLimit = MAX_RAND_NUM_HRD + 1; break;		// this question
		default: randNumLimit = MAX_RAND_NUM_EZ + 1; break;
	}
	/* All questions are generated using the same basic method: generate random values as needed by the question type, 
	   display the question with the numbers filled in, and calculate the correct answer to the question so that it
	   can be returned and used for verification */
	unsigned short answer;
	switch(type) {
		case Add_2 :
			do {
				n1 = GenRand() % randNumLimit;
				n2 = GenRand() % randNumLimit;
			} while ((n1 + n2) > MAX_ANSWER_VAL);
			LCD_Cursor(1);
			Custom_LCD_WriteNum(n1);
			LCD_WriteData('+');
			Custom_LCD_WriteNum(n2);
			answer = n1 + n2;
			break;
		case Add_3 :
			do {
				n1 = GenRand() % randNumLimit;
				n2 = GenRand() % randNumLimit;
				n3 = GenRand() % randNumLimit;
			} while ((n1 + n2 + n3) > MAX_ANSWER_VAL);
			LCD_Cursor(1);
			Custom_LCD_WriteNum(n1);
			LCD_WriteData('+');
			Custom_LCD_WriteNum(n2);
			LCD_WriteData('+');
			Custom_LCD_WriteNum(n3);
			answer = n1 + n2 + n3;
			break;
		case Sub_2 :
			n2 = GenRand() % randNumLimit/2;			// Right side
			if (n2 == 0) n2++;
			n1 = n2 + (GenRand() % randNumLimit/2);		// Left side (cannot generate a negative result)
			LCD_Cursor(1);
			Custom_LCD_WriteNum(n1);
			LCD_WriteData('-');
			Custom_LCD_WriteNum(n2);
			answer = n1 - n2;
			break;
		default: LCD_WriteData('X'); answer = 0; break;
	}
	return answer;
}
	
unsigned char IsDigit(unsigned char data) {
	return (data == '0') || (data == '1') ||
	 (data == '2') || (data == '3') ||
	 (data == '4') || (data == '5') ||
	 (data == '6') || (data == '7') ||
	 (data == '8') || (data == '9');
}

unsigned char Char2Digit(unsigned char data) {
	switch(data) {
		case '0' : return 0; break;
		case '1' : return 1; break;
		case '2' : return 2; break;
		case '3' : return 3; break;
		case '4' : return 4; break;
		case '5' : return 5; break;
		case '6' : return 6; break;
		case '7' : return 7; break;
		case '8' : return 8; break;
		case '9' : return 9; break;
		default: return 0;			// Should not happen; function is only called if the data
	}								// is known to be a single digit number as a character
}
#endif /* GAMEMASTER_H_ */