/*
 * March 2021, OST 
 * Blinks LED on PB0 with frequency of 2Hz
 * Uses timer0 interrupt
 * Timer0 runs with 8Mhz / 1024 -> 128us per tick
 * counts from 178 to 256 -> 78 * 128us = 9.98ms
 *
 * S1..S3 on PD4..PD6 are read
 * They form a number which is multiplied by 8
 * This result is added to the counter start (178)
 * The sum is loaded into the compare match register
 * OCR0A = 178 + (S1..S3) * 8 + 1
 * PB1 is set every 10ms, and reset when a match occurs
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

const char T0Start = 178;
const char counterStart = 25;
char counter;

// timer 0 interrupt service routine
ISR(TIMER0_OVF_vect) {	// once per 10ms
	TCNT0 = T0Start;
	PORTB = PINB | 2;
	counter--;
	if (!counter) {
		counter = counterStart;
		PORTB = PINB ^ 1;
	}
}

// timer 0 compare match A service routine
ISR(TIMER0_COMPA_vect) {
	PORTB = PINB & ~2;	
}

int main() {
	counter = counterStart;
	DDRB |= 3;
	TCCR0B = 5;		// prescale by 1024
	TCNT0 = T0Start;
	TIMSK0 = 3;		// enable overflow and compare match interrupts
	OCR0A = T0Start + 8;
	sei();

	while (1) {
		unsigned char in = (PIND & 0x70) >> 1;
		OCR0A = T0Start + 1 + in;
	}

	return 0;
}
