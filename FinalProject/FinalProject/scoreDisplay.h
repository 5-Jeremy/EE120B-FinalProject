#ifndef SCOREDISPLAY_H_
#define SCOREDISPLAY_H_

#define EEPROM_START_ADDRESS 0	// Scores occupy addresses 0-39

#define MEDAL_CODE 0
#define TROPHY_CODE 3

// Max score possible: 15*3*(500 + 1000 + 200) = 76500

void WriteScoreAndUpdateList (unsigned char pos, unsigned long score);
unsigned char GetScorePosition(unsigned long score);
unsigned long ReadScore(unsigned char pos);
void WriteScore(unsigned char pos, unsigned long score);
void WipeMemory();

enum SD_states {SD_start, SD_wait, SD_retrieve, SD_store, SD_wipe};
int SD_Tick(int state) {
	static unsigned char currReadNum;
	static unsigned char readPos;
	switch(state) {
		case SD_start :
			state = SD_wait;
			currReadNum = 0;
			scoreWritten = -1;
			break;
		case SD_wait :
			if (tryWriteScore) {
				state = SD_store;
			}
			else if (wipeScores) {
				state = SD_wipe;
			}
			else if ((scoreReadNum > 0) && (scoreReadNum <= 10) && (scoreReadNum != currReadNum) && inScoreMenu) {
				state = SD_retrieve;
				readPos = scoreReadNum;
				currReadNum = scoreReadNum;
				LCD_ClearScreen();
				LCD_Cursor(1);
			}
			else if (scoreReadNum == 0 && currReadNum != 0) {
				currReadNum = 0;
				LCD_ClearScreen();
			}
			else {
				state = SD_wait;
				readPos = 0;
			}
			if (returnToMenu) {
				scoreWritten = -1;
			}
			break;
		case SD_retrieve :
			state = SD_wait;
			LCD_WriteData('#');
			Custom_LCD_WriteNum(readPos);
			LCD_WriteData(' ');
			if (readPos <= 3) {
				LCD_WriteData(TROPHY_CODE);
			}
			else {
				LCD_WriteData(MEDAL_CODE);
			}
			LCD_Cursor(17);
			Custom_LCD_WriteNum_ul(ReadScore(readPos));
			break;
		case SD_store :
			state = SD_wait;
			unsigned char pos = GetScorePosition(score);
			if (pos) {
				WriteScoreAndUpdateList(pos,score);
				scoreWritten = pos;
			}
			else {
				scoreWritten = 0;
			}
			break;
		case SD_wipe :
			state = SD_wait;
			WipeMemory();
			break;
		default: state = SD_start;
	}
	return state;
}

void WriteScoreAndUpdateList(unsigned char pos, unsigned long score) {
	unsigned char i;
	unsigned long scoreToMove;
	if (pos == 0) return;		// 0 is an invalid position
	for (i = 10; i > pos; i--) {
		scoreToMove = ReadScore(i-1);
		WriteScore(i,scoreToMove);
	}
	WriteScore(pos,score);
}

unsigned char GetScorePosition(unsigned long score) {
	unsigned char i;
	for (i = 1; i <= 10; i++) {
		if (score > ReadScore(i)) {
			return i;
		}
		else if (score == ReadScore(i)) {
			return 0;	// No need to store duplicates
		}
	}
	return 0;
}

unsigned long ReadScore(unsigned char pos) {
	unsigned long retScore = 0;
	unsigned char i;
	unsigned char addr = EEPROM_START_ADDRESS + (pos-1)*4;	// Each score takes up 4 memory addresses
	for (i = 0; i < 4; i++) {
		retScore += (unsigned long)EEPROM_ReadByte(addr + i) << 8*i;	// Scores are stored with the least significant bits
	}																		// in the first of the four addresses
	return retScore;
}

void WriteScore(unsigned char pos, unsigned long score) {
	unsigned char i;
	unsigned char addr = EEPROM_START_ADDRESS + (pos-1)*4;
	unsigned char byteToStore;
	for (i = 0; i < 4; i++) {
		byteToStore = score & 0x000000FF;
		score = score >> 8;
		EEPROM_StoreByte(addr + i,byteToStore);
	}
}

void WipeMemory() {
	unsigned char i;
	for (i = 0; i < 40; i++) {
		EEPROM_StoreByte(i,0x00);
	}
}

#endif /* SCOREDISPLAY_H_ */