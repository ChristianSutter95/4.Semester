/*
 * March 2021, OST
 * initializes USB
 * sends state of dip switch to host
 * reflects characters received on usb back to host
 */ 


#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "usb.h"

FILE usbStream = FDEV_SETUP_STREAM(usbWriteChar, usbReadChar, _FDEV_SETUP_RW);

int main(void) {
	uint8_t prevIn = PIND;
//	char str[32];
	usbInit();	
	stdout = stdin = &usbStream;
	sei();
	
    while(1) {
		if (usbGetAvailableReceive() > 0) usbWriteChar(usbReadChar());
		if ((PIND & 0xf0) ^ prevIn) {
			prevIn = PIND & 0xf0;
// 			sprintf(str, "pind = 0x%x\r\n", prevIn);
// 			usbWriteString((uint8_t*)str);
			printf("pind = 0x%x\r\n", prevIn);
		}
	}
}