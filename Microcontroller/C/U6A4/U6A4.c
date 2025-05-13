/*
 * March 2021, OST 
 * Reads state of button on PD6. When state has changed 5 times the output on PB1 toggles
 */ 

#include <avr/io.h>

int main() {
	char count = 0;
	char in, prevIn;

	DDRB |= 0x02;
	DDRD &= ~0x40;
	prevIn = PIND;

	while (1) {
		in = PIND;
		if ((in ^ prevIn) & 0x40) {	// state changed
			count++;
			prevIn = in;
			if (count == 5) {
				PORTB = PINB ^ 0x02;
				count = 0;
			}
		}
	}

	return 0;
}
