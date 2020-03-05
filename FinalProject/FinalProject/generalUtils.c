// Returns the number of digits in num
unsigned char GetNumDigits_us(unsigned short num) {
	if (num < 10) {
		return 1;
	}
	else if (num < 100) {
		return 2;
	}
	else if (num < 1000) {
		return 3;
	}
	else if (num < 10000) {
		return 4;
	}
	else {					// unsigned short cannot have more than 5 digits
		return 5;
	}
}

// Fills the array str with characters corresponding to the digits in num
void IntToString(unsigned short num, unsigned char *str) {
	unsigned char numDigits = GetNumDigits_us(num);
	unsigned char i;
	for (i = 0; i < numDigits; i++) {
		str[numDigits - (i + 1)] = (num % 10) + '0';
		num /= 10;
	}
}