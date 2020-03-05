#ifndef KEYPADCTRL_H_
#define KEYPADCTRL_H_
// Contains the functions used by the keypadCtrl task

enum KC_states {KC_start, KC_main, KC_press};
int KC_Tick(int state) {
	switch (state) {
		case KC_start :
			state = KC_main;
			lastPressed = '\0';
			keyDown = 0;
			break;
		case KC_main :
			state = (GetKeypadKey() == '\0') ? KC_main : KC_press;
			if (state == KC_press) {
				lastPressed = GetKeypadKey();
				keyDown = 1;
			}
			break;
		case KC_press :
			state = (GetKeypadKey() == '\0') ? KC_main : KC_press;
			if (state == KC_main) {
				keyDown = 0;
			}
			break;
		default: state = KC_start; break;
	}
	return state;
}

#endif /* KEYPADCTRL_H_ */