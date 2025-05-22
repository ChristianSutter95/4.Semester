/*
 * April 2021, OST 
 * reads ADC value once per second
 * calculates temperature within 5 degrees
 * outputs val to USB
 * uses sleep mode
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "usb.h"

volatile uint8_t go;
uint16_t const table[13] = {75, 100, 131, 169, 214, 265, 322, 384, 447, 512, 574, 633, 687};

ISR(ADC_vect) {go = 1;}
ISR(TIMER3_COMPA_vect) {go = 1;}

int main() {
	uint8_t str[32], i;
	uint16_t val;
	go = 0;	

	usbInit();

	// init timer 3, interrupts every second
	TCCR3A = 0;
	TCCR3B = 0xc;	// CTC mode, prescaler = 256+
	OCR3A = 31250;	// gives exactly one second
	TIMSK3 |= 0x02;	// enable output compare match ints
	
	// init ADC
	ADCSRA = 0x0f;	// enable ints, prescaler = 128 
	ADMUX = 0x40;	// AVcc

	sei();

	while (1) {
		set_sleep_mode(SLEEP_MODE_IDLE);
		ADCSRA &= ~0x80;	// disable ADC
		while (!go) sleep_mode();
		go = 0;

		ADCSRA |= 0x80;		// enable ADC	
		set_sleep_mode(SLEEP_MODE_ADC);
		while (!go) sleep_mode();
		go = 0;

		i = 0;
		while (ADCW > table[i]) i++;
		sprintf(str, "Temperature: %4d\r\n", -25 + 5*i);	// print temperature
		usbWriteString(str);	
	}

	return 0;
}
