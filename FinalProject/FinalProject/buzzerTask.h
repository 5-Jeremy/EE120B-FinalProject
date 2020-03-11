#ifndef BUZZERTASK_H_
#define BUZZERTASK_H_
// Contains the functions used by the buzzerTask task

#define TIME_MAX (600 + (difficulty*200))
#define WARN_FREQ_1_1 300
#define WARN_FREQ_1_2 350
#define WARN_FREQ_2_1 400
#define WARN_FREQ_2_2 450
#define WARN_FREQ_3_1 500
#define WARN_FREQ_3_2 550
#define INCORRECT_FREQ 600

enum BT_states {BT_start, BT_wait, BT_warn1, BT_warn2, BT_warn3, BT_incorrect};
int BT_Tick(int state) {
	static unsigned char i;
	switch (state) {
		case BT_start :
			state = BT_wait;
			PWM_on();
			break;
		case BT_wait :
			PWM_off();
			if (!gameInProgress) {
				state = BT_wait;
				set_PWM(0);
			}
			else if (!QueueIsEmpty(buzzQ)) {
				QueueDequeue(buzzQ);
				state = BT_incorrect;
				PWM_on();
				i = 0;
			}
			else if (timeLeft > 0 && timeLeft <= TIME_MAX/2) {
				state = BT_warn1;
				PWM_on();
				i = 0;
			}
			else {
				state = BT_wait;
				set_PWM(0);
			}
			break;
		case BT_warn1 :
			if (!gameInProgress) {
				state = BT_wait;
				set_PWM(0);
			}
			else if (!QueueIsEmpty(buzzQ)) {
				QueueDequeue(buzzQ);
				state = BT_incorrect;
				i = 0;
			}
			else if (timeLeft > (TIME_MAX*3)/10 && timeLeft <= TIME_MAX/2) {
				if (i <= 15) {
					set_PWM(WARN_FREQ_1_1);
				}
				else {
					set_PWM(WARN_FREQ_1_2);
					if (i >= 30) i = 0;
				}
				i++;
			}
			else {
				state = BT_warn2;
				set_PWM(WARN_FREQ_2_1);
				i = 0;
			}
			break;
		case BT_warn2 :
			if (!gameInProgress) {
				state = BT_wait;
				set_PWM(0);
			}
			else if (!QueueIsEmpty(buzzQ)) {
				QueueDequeue(buzzQ);
				state = BT_incorrect;
				i = 0;
			}
			else if (timeLeft > TIME_MAX/6 && timeLeft <= (TIME_MAX*3)/10) {
				if (i <= 10) {
					set_PWM(WARN_FREQ_2_1);
				}
				else {
					set_PWM(WARN_FREQ_2_2);
					if (i >= 20) i = 0;
				}
				i++;
			}
			else {
				state = BT_warn3;
				set_PWM(WARN_FREQ_3_1);
				i = 0;
			}
			break;
		case BT_warn3 :
			if (!gameInProgress) {
				state = BT_wait;
				set_PWM(0);
			}
			else if (!QueueIsEmpty(buzzQ)) {
				QueueDequeue(buzzQ);
				state = BT_incorrect;
				i = 0;
			}
			else if (timeLeft > 0 && timeLeft <= TIME_MAX/6) {
				if (i <= 5) {
					set_PWM(WARN_FREQ_3_1);
				}
				else {
					set_PWM(WARN_FREQ_3_2);
					if (i >= 10) i = 0;
				}
				i++;
			}
			else {
				state = BT_wait;
				set_PWM(0);
				i = 0;
			}
			break;
		case BT_incorrect :
			if (!gameInProgress) {
				state = BT_wait;
				set_PWM(0);
			}
			else if (timeLeft == 0) {
				state = BT_wait;
				set_PWM(0);
			}
			else if (i >= 40) {
				if (timeLeft > (TIME_MAX*3)/10 && timeLeft <= TIME_MAX/2) {
					 state = BT_warn1;
					 i = 0;
				}
				else if (timeLeft > TIME_MAX/6 && timeLeft <= (TIME_MAX*3)/10) {
					state = BT_warn2;
					i = 0;
				}
				else if (timeLeft > 0 && timeLeft <= TIME_MAX/6) {
					state = BT_warn3;
					i = 0;
				}
				else {
					state = BT_wait;
					set_PWM(0);
				}
			}
			else {
				state = BT_incorrect;
				i++;
				set_PWM(INCORRECT_FREQ);
			}
			break;
		default: state = BT_start; break;
	}
	return state;
}


#endif /* BUZZERTASK_H_ */