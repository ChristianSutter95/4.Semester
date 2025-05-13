/*
 * April 2021, OST
 * reads ADC value once per second
 * calculates temperature within 5 degrees
 * outputs val to USB
 */

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "usb.h"

uint8_t go;
uint16_t const table[13] = {75, 100, 131, 169, 214, 265, 322, 384, 447, 512, 574, 633, 687};
	
ISR(TIMER3_COMPA_vect) {go = 1;}

int main() {
	uint8_t str[32], i;
	
	usbInit();

	// init timer 3, interrupts every second
	TCCR3A = 0;
	TCCR3B = 0xc;	// CTC mode, prescaler = 256+
	OCR3A = 31250;	// gives exactly one second
	TIMSK3 |= 0x02;	// enable output compare match ints
 
	// init ADC
	ADCSRA = 0x87;	// enable adc, prescaler = 128
	ADMUX = 0x40;	// AVcc

	sei();

	while (1) {
		while (!go);
		go = 0;
		ADCSRA |= 0x40;							// start single conversion
		while ((ADCSRA & 0x40) != 0);			// wait for conversion to complete
//		sprintf(str, "ADC0: %4d\r\n", ADCW);	// print ADC value
		i = 0;
		while (ADCW > table[i]) i++;
		sprintf((char*)str, "Temperature: %4d\r\n", -25 + 5*i);	// print temperature
		usbWriteString(str);
	}
	return 0;
}
