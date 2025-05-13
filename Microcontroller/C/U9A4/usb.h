#ifndef USB_H_
#define USB_H_

/*
 * March 2021, OST 
 * Definitions for USB driver
 */ 

#include <stdint.h>

#define txBufLen 64
#define rxBufLen 32

void usbWriteChar(const uint8_t);
void usbWriteString(const uint8_t []);
uint8_t usbReadChar();
void usbReadString(uint8_t []);
uint16_t usbGetAvailableReceive();
void usbInit();

#endif /* USB_H_ */
