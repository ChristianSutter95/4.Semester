/*
 * U8A1.c
 *
 * Created: 13.05.2025 09:35:18
 * Author : chris
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "usb_common.h"

int main(void) {
	usbCommonInit();
	sei();
	while(1);
}

