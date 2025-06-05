#ifndef USB_H_
#define USB_H_

/*
 * March 2021, OST 
 * Definitions for USB driver
 */ 

#include <stdint.h>   // Standard-Datentypen (z.?B. uint8_t, uint16_t)

#define txBufLen 64   // Gr��e des Sende-Ringbuffers
#define rxBufLen 32   // Gr��e des Empfangs-Ringbuffers

// Schreibt ein einzelnes Zeichen in den Sende-Ringbuffer
void usbWriteChar(const uint8_t);

// Schreibt einen String (nullterminiert) in den Sende-Ringbuffer
void usbWriteString(const uint8_t []);

// Liest ein Zeichen aus dem Empfangs-Ringbuffer
uint8_t usbReadChar();

// Liest alle verf�gbaren Zeichen als String (nullterminiert)
void usbReadString(uint8_t []);

// Gibt Anzahl verf�gbarer empfangener Zeichen zur�ck
uint16_t usbGetAvailableReceive();

// Initialisiert USB-System und Ringbuffer
void usbInit();

#endif /* USB_H_ */
