#ifndef SEEDCHANGE_H_
#define SEEDCHANGE_H_
// Contains the functions used by the seedChange task

enum SC_states {SC_start, SC_wait, SC_newGameSelect, SC_newGameConfirmed};
int SC_Tick(int state) {
	static unsigned short i;
	static unsigned short d1; //------------------
	switch (state) {
		case SC_start :
			state = SC_wait;
			i = 0;		//------------------
			GenNewSeed(0);		// Reset seed to 0
			break;
		case SC_wait :
			state = newGameSelected ? SC_newGameSelect : SC_wait;
			if (state == SC_newGameSelect) {
				d1 = i;	//------------------
				i = 0;
			}
			i++;	//------------------
			break;
		case SC_newGameSelect :
			state = gameInProgress ? SC_newGameConfirmed : SC_newGameSelect;
			if (state == SC_newGameConfirmed) {
				GenNewSeed(i + d1); //------------------
			}
			i++;
			break;
		case SC_newGameConfirmed :
			state = gameInProgress ? SC_newGameConfirmed : SC_wait;
			break;
		default: state = SC_start; break;
	}
	return state;
}


#endif /* SEEDCHANGE_H_ */