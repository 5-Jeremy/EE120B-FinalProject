/*	Author: Jeremy Carleton jcarl023@ucr.edu
 *	Lab Section: 021
 *	Assignment: Custom Lab Project
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#define F_CPU 8000000UL
// Provided headers
#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.h"
#include "keypad.h"
#include "scheduler.h"
#include "timer.h"
#include "PWM.h" // Code taken from Lab 9
#include "queue.h"
// Custom headers
#include "generalUtils.h"
#include "LCD_Custom.h"
#include "EEPROM.h"
#include "random.h"
#include "nokia5110.h"

// Custom character codes
#define MEDAL_CODE 0
#define SQRT_CODE 1
#define ARROW_CODE 2
#define TROPHY_CODE 3

// Control which tasks to enable (for testing)
#define TASK_0_ENABLE 1	// KC
#define TASK_1_ENABLE 1	// IH
#define TASK_2_ENABLE 1	// SC
#define TASK_3_ENABLE 1	// MC
#define TASK_4_ENABLE 1	// GM
#define TASK_5_ENABLE 1	// DA
#define TASK_6_ENABLE 1	// SD
#define TASK_7_ENABLE 1	// BT
#define TASK_8_ENABLE 1	// LA

// Main timer period
unsigned long GCD = 10;

// -------- Shared Variables -------------
char lastPressed;				// <keypadCtrl> Stores the last char returned by GetKeypadKey()
unsigned char keyDown;			// <keypadCtrl> Indicates whether a key is being pressed
unsigned char newGameSelected;	// <menuCtrl> Indicates whether the new game option has been selected in the menu
unsigned char gameInProgress;	// <menuCtrl> Indicates whether the game is in progress
unsigned char returnToMenu;		// <menuCtrl> Indicates whether the player has returned to the menu after finishing a game
enum difficulty_types {EASY, MEDIUM, HARD} difficulty;	// <menuCtrl> The difficulty of the game
unsigned long currAnswer;		// <gameMaster> The current answer given by the player to the current question
unsigned char isInputAvailable;	// <gameMaster> Indicates whether there is any number to display for the user's answer
unsigned long score;			// <gameMaster> The score for the current set
unsigned char numLives;			// <gameMaster> The number of lives that the player has remaining
unsigned char problemsComplete;	// <gameMaster> The number of questions answered in the current set
unsigned char problemsLeft;		// <gameMaster> The number of questions left in the current set
unsigned char gameFin;			// <gameMaster> Indicates that the game has just ended
unsigned char gameLost;			// <gameMaster> Indicates that the game was lost (score will not be saved)
unsigned short timeLeft;		// <gameMaster> The time remaining to answer the current question
unsigned char screenUpdate;		// <gameMaster> Indicates that the Nokia LCD's screen needs to be updated during
									// a game, allowing the screen to only be redrawn when necessary
									
// Shared variables used for interacting with the EEPROM
unsigned char scoreReadNum;		// <menuCtrl> The number of the score to be displayed (1 is the highest); 0 if no read needed
char scoreWritten;				// <scoreDisplay> Indicates whether or not the score was saved at the end of a game; 
									// -1 indicates the score has not been checked yet, 0 indicates the score was not saved,
									// 1-10 indicates that the score was saved to that location;
unsigned char wipeScores;		// <menuCtrl> Indicates that the user has chosen to wipe all scores from the EEPROM
unsigned char tryWriteScore;	// <menuCtrl> Tells scoreDisplay to write the new score to the EEPROM if needed
unsigned char inScoreMenu;		// <menuCtrl> Indicates whether or not the highscores menu is open
// -------- End Shared Variables -------------

// -------- Queues -------------
Queue menuQ;	// inputHandler -> menuCtrl
Queue gameQ;	// inputHandler -> GameMaster
Queue buzzQ;	// GameMaster	-> buzzerTask
// -------- End Queues -------------

// Task Functions
#include "keypadCtrl.h"		// Task 0
#include "inputHandler.h"	// Task 1
#include "seedChange.h"		// Task 2
#include "menuCtrl.h"		// Task 3
#include "GameMaster.h"		// task 4
#include "displayAnswer.h"	// task 5
#include "scoreDisplay.h"	// task 6
#include "buzzerTask.h"		// task 7
#include "LEDArray.h"		// task 8

int main(void) 
{
	// Set Data Direction Registers
	DDRA = 0xFF; PORTA = 0x00;	// LED Array: PA0-4 LCD Control Bus: PA6-7
    DDRB = 0x7F; PORTB = 0x80;	// Nokia: PB0-PB4 Buzzer: PB6 Reset: PB7
    DDRC = 0xFF; PORTC = 0x00;	// LCD Data Bus
    DDRD = 0xF0; PORTD = 0x0F;	// Keypad
	// Initialize queues
	menuQ = QueueInit(1);
	gameQ = QueueInit(1);
	buzzQ = QueueInit(1);
	// Set up task scheduler
	static task task0,task1,task2,task3,task4,task5,task6,task7,task8;
	task* tasks[] = {&task0,&task1,&task2,&task3,&task4,&task5,&task6,&task7,&task8};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	unsigned char taskEnableList[9] = {TASK_0_ENABLE, TASK_1_ENABLE, TASK_2_ENABLE, TASK_3_ENABLE, TASK_4_ENABLE, TASK_5_ENABLE, TASK_6_ENABLE,
										TASK_7_ENABLE, TASK_8_ENABLE};

	// Task 0 (keypadCtrl)
	/* Monitors whether a key on the keypad is being pressed, and updates lastPressed
		with the corresponding char */
	task0.state = KC_start;
	task0.period = GCD;
	task0.elapsedTime = task0.period;
	task0.TickFct = &KC_Tick;
	// Task 1 (inputHandler)
	/* Uses the menuQ and gameQ queues to communicate user input to the relevant task */
	task1.state = IH_start;
	task1.period = GCD;
	task1.elapsedTime = task1.period;
	task1.TickFct = &IH_Tick;
	// Task 2 (seedChange)
	/* Generates and controls the seed used to generate random numbers for the game
		using a linear congruential generator */
	task2.state = SC_start;
	task2.period = GCD;
	task2.elapsedTime = task2.period;
	task2.TickFct = &SC_Tick;
	// Task 3 (menuCtrl)
	/* Controls what data is displayed on the Nokia 5110 LCD display, and responds
		to user choices when in a menu */
	task3.state = MC_start;
	task3.period = GCD;
	task3.elapsedTime = task3.period;
	task3.TickFct = &MC_Tick;
	// Task 4 (GameMaster)
	/* Controls the generation of questions and tracks the user's answer. Also keeps 
		track of time remaining, score, and number of lives */
	task4.state = GM_start;
	task4.period = GCD;
	task4.elapsedTime = task4.period;
	task4.TickFct = &GM_Tick;
	// Task 5 (displayAnswer)
	/* Displays the current answer on the 16x2 LCD */
	task5.state = DA_start;
	task5.period = GCD;
	task5.elapsedTime = task5.period;
	task5.TickFct = &DA_Tick;
	// Task 6 (scoreDisplay)
	/* Stores the score at the end of the game if it is higher than at least one of the currently saved scores,
		or there are less than 10 scores currently saved. Also reads and displays scores when in the highscores
		menu */
	task6.state = SD_start;
	task6.period = GCD;
	task6.elapsedTime = task6.period;
	task6.TickFct = &SD_Tick;
	// Task 7 (buzzerTask)
	/* Controls the PWM signal that activates the buzzer to alert the player that time is running out or indicate
		that an incorrect answer was given */
	task7.state = BT_start;
	task7.period = GCD;
	task7.elapsedTime = task7.period;
	task7.TickFct = &BT_Tick;
	// Task 8 (LEDArray)
	/* Controls the LED array used to visually indicate time remaining on a certain problem */ 
	task8.state = LA_start;
	task8.period = GCD;
	task8.elapsedTime = task8.period;
	task8.TickFct = &LA_Tick;
	// Initialize timer and components
	TimerSet(GCD);
	TimerOn();
	Custom_LCD_Init();
	LCD_ClearScreen();
	LCD_Cursor(1);
	nokia_lcd_init();
	nokia_lcd_clear();
	// Scheduler code from Lab 11, with modifications
	unsigned short i; //scheduler for loop
	while (1) {
		if (!(PINB & 0x80)) {			// When reset button is pressed, all tasks return to their start state
			for(i=0; i<numTasks;i++){
				tasks[i]->elapsedTime = 0;
				tasks[i]->state = 0;
			}
			LCD_ClearScreen();
		}
		for(i=0; i<numTasks;i++){
			if (taskEnableList[i]) {
				if (tasks[i]->elapsedTime == tasks[i]->period){ //task ready to tick
					tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);// Set Next state
					tasks[i]->elapsedTime=0; //Reset the elapsed time
				}
				tasks[i]->elapsedTime += GCD;
			}
		}
		while(!TimerFlag);
		TimerFlag = 0;
	}
	
	return 0;
}