/*
 * March 2024, OST 
 * Blinks LED on PB0 with frequency of 2Hz
 * Uses interrupts
 * Timer0 runs with 8Mhz / 1024 -> 128us per tick
 * counts from 178 to 256 -> 78 * 128us = 9.98ms
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer0.h"

const char T0Start = 178;
const char counterStart = 25;
char counter;

// timer 0 interrupt service routine
ISR(TIMER0_OVF_vect) {	// once per 10ms
	timer0setCount(T0Start);
	counter--;
	if (!counter) {
		counter = counterStart;
		PORTB = PINB ^ 1;
	}
}

void timer0Init() {
	counter = counterStart;
	TCCR0B = 5;		// prescale by 1024
	timer0setCount(T0Start);
	TIMSK0 |= 1;
}

unsigned char timer0getCount() {
	return TCNT0;
}

void timer0setCount(unsigned char count) {
	TCNT0 = count;
}
