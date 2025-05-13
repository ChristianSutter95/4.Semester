/*
 * March 2021, OST 
 * Reads state of button on PD6 and output this state on PB1.
 */ 

#include <avr/io.h>

int main() {

	DDRB |= 0x02;
	DDRD &= ~0x40;

	while (1) {
		char in = PIND;
		if (in & 0x40) PORTB = PINB | 0x02;
		else PORTB = PINB & ~0x02;
	}

	return 0;
}
