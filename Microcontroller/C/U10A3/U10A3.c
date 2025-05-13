/*
 * April 2021, OST
 * initializes USB
 * sends state of dip switch to host in the form of "switch[1|2|3|4] [pressed|released]
 * receives commands from the host in the form of "led[1|2|3|4] [on|off] and actions on the led's accordingly
 * function "countChar" returns number of occurrences of a given character in a string
 */ 


#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "usb.h"

uint8_t countChar(uint8_t str[], uint8_t ch) {
	uint8_t i = 0, count = 0;
	while (str[i] != 0) {
		if (str[i] == ch) count++;
		i++;
	}
	return count;
}
int main(void) {
	uint8_t prevIn = PIND, k = 0;
	char str[32], outStr[32];
	DDRB |= 0xf;
	usbInit();	
	sei();
	
    while(1) {
		uint8_t ch;
		if (usbGetAvailableReceive() > 0) {
			ch = usbReadChar();
			if (ch == '\r') {
				str[k] = 0;
				if (strstr(str, "led")) {
					uint8_t pin = str[3] - '1';
					if (strstr(str, "on")) PORTB |= 1 << pin;
					if (strstr(str, "off")) PORTB &= ~(1 << pin);
				}
				k = 0;
			} else 
				str[k++] = ch;
		}
		uint8_t in = (PIND & 0xf0) ^ prevIn;
		if (in) {
			uint8_t pin = 0; 
			while (!(in & (1 << pin))) pin++;
			if (PIND & (1 << pin))
 				sprintf(outStr, "switch %d released\r\n", pin-3);
			else
  				sprintf(outStr, "switch %d pressed\r\n", pin-3);
			usbWriteString((uint8_t*)outStr);
			prevIn = PIND & 0xf0;
		}
	}
}
