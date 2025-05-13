/*
 * March 2021, OST 
 * Reads states of buttons on PD4..PD7 and outputs this state on PB0..PB3
 */ 

#include <avr/io.h>

int main(void) {
	char state;

	DDRB |= 0x0f;
	DDRD = 0;

	while (1) {
		state = PIND;
		PORTB = state >> 4;
	}
}