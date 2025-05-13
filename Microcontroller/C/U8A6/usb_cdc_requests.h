#ifndef USB_CDC_REQUESTS_H_
#define USB_CDC_REQUESTS_H_

/*
 * Definitions for USB CDC descriptors.
 * 6.4.2016, Urs Graf
 */

#include "config.h"

//**********      CDC requests      **********//
#define SET_LINE_CODING				0x20
#define GET_LINE_CODING				0x21
#define SET_CONTROL_LINE_STATE		0x22

//**********      Class specific request types      **********//
#define CDC_WRITE_REQUEST			0x21
#define CDC_READ_REQUEST			0xA1

//**********      Functions      **********//
uint8_t usbCDCRequest(uint8_t, uint8_t);
uint8_t usbDeviceGetDescriptor(uint8_t, uint8_t);
void usbDeviceEndpointInit(uint8_t);

#endif /*USB_CDC_REQUESTS_H_*/
