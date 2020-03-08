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

// Other macros
#define EEPROM_START_ADDRESS 10

// Control which tasks to enable
#define TASK_0_ENABLE 1
#define TASK_1_ENABLE 1
#define TASK_2_ENABLE 1
#define TASK_3_ENABLE 1
#define TASK_4_ENABLE 1
#define TASK_5_ENABLE 1

// Main timer period
unsigned long GCD = 10;

// -------- Shared Variables -------------
char lastPressed;				// <keypadCtrl> Stores the last char returned by GetKeypadKey()
unsigned char keyDown;			// <keypadCtrl> Indicates whether a key is being pressed
unsigned char newGameSelected;	// <menuCtrl>
unsigned char gameInProgress;	// <menuCtrl>
unsigned char buzzer;			// <menuCtrl>
enum difficulty_types {EASY, MEDIUM, HARD} difficulty;	// <menuCtrl>
unsigned long currAnswer;		// <gameMaster>
unsigned char isInputAvailable;	// <gameMaster>
unsigned char numAnswerDigits;	// <gameMaster>
unsigned char score;			// <gameMaster>
unsigned char timeLeft;			// <gameMaster>
// -------- End Shared Variables -------------

// -------- Queues -------------
Queue menuQ;	// inputHnadler -> menuCtrl
Queue gameQ;	// inputHandler -> GameMaster
// -------- End Queues -------------

// Task Functions
#include "keypadCtrl.h"		// Task 0
#include "inputHandler.h"	// Task 1
#include "seedChange.h"		// Task 2
#include "menuCtrl.h"		// Task 3
#include "GameMaster.h"		// task 4
#include "displayAnswer.h"	// task 5


int main(void) 
{
	DDRA = 0xFE; PORTA = 0x01;	// LCD Control Bus
    DDRB = 0x7F; PORTB = 0x80;	// Nokia: PB0-PB4 Reset: PB7
    DDRC = 0xFF; PORTC = 0x00;	// LCD Data Bus
    DDRD = 0xF0; PORTD = 0x0F;	// Keypad
	
	menuQ = QueueInit(1);
	gameQ = QueueInit(1);
	
	static task task0,task1,task2,task3,task4,task5;
	task* tasks[] = {&task0,&task1,&task2,&task3,&task4,&task5};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	unsigned char taskEnableList[6] = {TASK_0_ENABLE, TASK_1_ENABLE, TASK_2_ENABLE, TASK_3_ENABLE, TASK_4_ENABLE, TASK_5_ENABLE};

	// Task 0 (keypadCtrl)
	task0.state = KC_start;
	task0.period = GCD;
	task0.elapsedTime = task0.period;
	task0.TickFct = &KC_Tick;
	// Task 1 (inputHandler)
	task1.state = IH_start;
	task1.period = GCD;
	task1.elapsedTime = task1.period;
	task1.TickFct = &IH_Tick;
	// Task 2 (seedChange)
	task2.state = SC_start;
	task2.period = GCD;
	task2.elapsedTime = task2.period;
	task2.TickFct = &SC_Tick;
	// Task 3
	task3.state = MC_start;
	task3.period = GCD;
	task3.elapsedTime = task3.period;
	task3.TickFct = &MC_Tick;
	// Task 4
	task4.state = GM_start;
	task4.period = GCD;
	task4.elapsedTime = task4.period;
	task4.TickFct = &GM_Tick;
	// Task 5
	task5.state = DA_start;
	task5.period = GCD;
	task5.elapsedTime = task5.period;
	task5.TickFct = &DA_Tick;
	
	TimerSet(GCD);
	TimerOn();
	Custom_LCD_Init();
	LCD_ClearScreen();
	LCD_Cursor(1);
	nokia_lcd_init();
	nokia_lcd_clear();

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