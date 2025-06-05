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

// Sende-Ringbuffer: Pufferarray, Leseposition (out), Anzahl Zeichen (len)
struct {
	uint8_t buf[txBufLen];
	uint16_t out, len;
} txBuf;

// Empfangs-Ringbuffer: analog wie txBuf
struct {
	uint8_t buf[rxBufLen];
	uint16_t out, len;
} rxBuf;

// Schreibt ein Zeichen in den Sende-Ringbuffer
void usbWriteChar(const uint8_t ch) {
	uint8_t status = SREG;		// Aktuellen Status sichern
	cli();                      // Interrupts sperren (atomarer Zugriff)
	if (txBuf.len < txBufLen) {
		// "in"-Position berechnen und Zeichen speichern
		txBuf.buf[(txBuf.out + txBuf.len) % txBufLen] = ch;
		txBuf.len++;            // Länge erhöhen
	} 
	SREG = status;              // Ursprünglichen Interruptstatus wiederherstellen
}

// Schreibt einen String zeichenweise in den Sende-Ringbuffer
void usbWriteString(const uint8_t str[]) {
	uint8_t i = 0;
	while (str[i] != 0) {       // Bis Nullzeichen erreicht
		usbWriteChar(str[i++]);
	}
}
 
// Liest ein Zeichen aus dem Empfangs-Ringbuffer
uint8_t usbReadChar(void) {
	uint8_t status = SREG;		// Status sichern
	cli();                      // Interrupts sperren (atomar)
	uint8_t ch = 0;
	if (rxBuf.len > 0) {
		// Zeichen lesen und Position/Länge anpassen
		ch = rxBuf.buf[rxBuf.out];
		rxBuf.out = (rxBuf.out + 1) % rxBufLen;
		rxBuf.len--;
	}
	SREG = status;              // Ursprünglichen Status zurücksetzen
 	return ch;
}

// Liest alle Zeichen aus Empfangsbuffer und schreibt sie in String (nullterminiert)
void usbReadString(uint8_t str[]) {
	while (rxBuf.len > 0) {
		*str++ = usbReadChar();    // Zeichen für Zeichen kopieren
	}
	*str = 0;                      // Nullterminierung
}

// Gibt aktuelle Anzahl empfangener Zeichen zurück
uint16_t usbGetAvailableReceive() {
	return rxBuf.len;
}

// Initialisiert USB-System (inkl. EPs, PLL etc.)
void usbInit() {
	usbCommonInit();	
}
