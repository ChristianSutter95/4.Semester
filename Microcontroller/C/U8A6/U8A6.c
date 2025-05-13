/*
 * March 2021, OST
 * Configures USB as CDC device
 * sends 'a' per 1000 requests from the master, 'b' if button 3 pressed
 * lights led 0 upon receiving 'x', switch it off with any other received character
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "usb_common.h"


int main(void) {
	DDRB |= 1;
	usbCommonInit();	
	sei();
	
    while(1);
}