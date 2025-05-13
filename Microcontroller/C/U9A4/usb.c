/*
 * April 2023, OST
 * Driver for USB
 * Contains ring buffers for receiving and transmitting 
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "usb_common.h"
#include "usb.h"

// declarations for transmit and receive buffers

struct {
	uint8_t buf[txBufLen];
	uint16_t out, len;
} txBuf;

struct {
	uint8_t buf[rxBufLen];
	uint16_t out, len;
} rxBuf;

// transmits a single byte over USB
void usbWriteChar(const uint8_t ch) {
	uint8_t status = SREG;		// make atomic
	cli();
	if (txBuf.len < txBufLen) {
		txBuf.buf[(txBuf.out + txBuf.len) % txBufLen] = ch;
		txBuf.len++;
	} 
	SREG = status;
}

// transmits a string over USB
void usbWriteString(const uint8_t str[]) {
	uint8_t i = 0;
	while (str[i] != 0) {
		usbWriteChar(str[i++]);
	}
 }
 
 // reads a single character over USB
 uint8_t usbReadChar(void) {
	 uint8_t status = SREG;		// make atomic
	 cli();
	 uint8_t ch = 0;
	 if (rxBuf.len > 0) {
		 ch = rxBuf.buf[rxBuf.out];
		 rxBuf.out = (rxBuf.out + 1) % rxBufLen;
		 rxBuf.len--;
	 }
	 SREG = status;
 	 return ch;
}

 // reads all available characters from USB and returns them as a string
void usbReadString(uint8_t str[]) {
	while (rxBuf.len > 0) {
		*str++ = usbReadChar();
	}
	*str = 0; // terminate the string
}

// returns the number of available characters in the receive buffer
uint16_t usbGetAvailableReceive() {
	return rxBuf.len;
}

// initializes the USB interface
void usbInit() {
	usbCommonInit();	
}


