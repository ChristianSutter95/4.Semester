/*
 * March 2021, OST 
 * Blinks LED on PB0 with frequency of 2Hz
 * Uses timer0 interrupt
 * Timer0 runs with 8Mhz / 1024 -> 128us per tick
 * counts from 178 to 256 -> 78 * 128us = 9.98ms
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

const char T0Start = 178;
const char counterStart = 25;
char counter;

// timer 0 interrupt service routine
ISR(TIMER0_OVF_vect) {	// once per 10ms
	TCNT0 = T0Start;
	counter--;
	if (!counter) {
		counter = counterStart;
		PORTB = PINB ^ 1;
	}
}

int main() {
	counter = counterStart;
	DDRB |= 7;
	TCCR0B = 5;		// prescale by 1024
	TCNT0 = T0Start;
	TIMSK0 = 1;		// enable overflow interrupt
	sei();

	while (1);

	return 0;
}
