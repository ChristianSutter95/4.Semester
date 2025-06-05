/*
 * March 2024, OST 
 * Uses timer 0 to blink LED on PB0 with frequency of 2Hz
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer0.h"

int main() {
	DDRB |= 1;
	timer0Init();
	sei();

	while (1);

	return 0;
}

