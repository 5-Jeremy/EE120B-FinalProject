#ifndef LEDARRAY_H_
#define LEDARRAY_H_
// Contains the functions used by the LEDArray task

#define TIME_MAX (600 + (difficulty*200))

enum LA_states {LA_start, LA_wait, LA_gameInProgress};
int LA_Tick(int state) {
	unsigned char LEDSequence;
	switch(state) {
		case LA_start :
			state = LA_wait;
			LEDSequence = 0;  
			break;
		case LA_wait :
			state = (gameInProgress && timeLeft > 0) ? LA_gameInProgress : LA_wait;
			LEDSequence = 0;
			break;
		case LA_gameInProgress :
			state = (gameInProgress && timeLeft > 0) ? LA_gameInProgress : LA_wait;   
			if (timeLeft >= (4*TIME_MAX)/5) {
				LEDSequence = 0x01F;
			}                       
			else if (timeLeft >= (3*TIME_MAX)/5) {
				LEDSequence = 0x0F;
			}
			else if (timeLeft >= (2*TIME_MAX)/5) {
				LEDSequence = 0x07;
			}
			else if (timeLeft >= (1*TIME_MAX)/5) {
				LEDSequence = 0x03;
			}
			else if (timeLeft > 0) {
				LEDSequence = 0x01;
			}
			else {
				LEDSequence = 0;
			}
			PORTA = (PORTA & 0xE0) | LEDSequence;
			break;
		default: state = LA_start; break;
	}
	return state;
}

#endif /* LEDARRAY_H_ */