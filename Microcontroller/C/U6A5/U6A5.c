/*
 * March 2021, OST 
 * Reads state of button on PD6. When state has changed 3 times to positive level the output on PB1 toggles
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
			prevIn = in;
			if (in & 0x40) { // state is positive
				count++;
				if (count == 3) {
					PORTB = PINB ^ 0x02;
					count = 0;
				}
			}
		}
	}

	return 0;
}
