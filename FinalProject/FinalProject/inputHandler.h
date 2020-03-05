#ifndef INPUTHANDLER_H_
#define INPUTHANDLER_H_
// Contains the functions used by the inputHandler task

enum IH_states {IH_start, IH_noPress, IH_waitRelease};
int IH_Tick(int state) {
	switch(state) {
		case IH_start :
			state = IH_noPress;
			QueueMakeEmpty(menuQ);
			QueueMakeEmpty(gameQ);
			break;
		case IH_noPress :
			if (keyDown) {
				state = IH_waitRelease;
				if (!gameInProgress) {
					QueueEnqueue(menuQ, lastPressed);
				}
				else {
					QueueEnqueue(gameQ, lastPressed);
				}
			}
			else {
				state = IH_noPress;
			}
			break;
		case IH_waitRelease :
			if (!keyDown) {
				state = IH_noPress;
			}
			else {
				state = IH_waitRelease;
			}
			break;
		default: state = IH_start; break;
	}
	return state;
}


#endif /* INPUTHANDLER_H_ */