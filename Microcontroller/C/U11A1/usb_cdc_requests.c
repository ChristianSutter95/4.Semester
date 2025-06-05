/*
 * Contains functions to handle CDC (communication device class) specific requests.
 * 1.4.2016, Urs Graf
 */

#include "usb_cdc_requests.h"
#include "usb_common.h"
#include "usb_requests.h"

void setLineCoding() {
	uint16_t wValue, wIndex, wLength;
	uint32_t baudrate;
	
	LSB(wValue) = USB_read_byte();	// is 0
	MSB(wValue) = USB_read_byte();
	LSB(wIndex) = USB_read_byte();	// read interface
	MSB(wIndex) = USB_read_byte();
	LSB(wLength) = USB_read_byte();	// read lenght, should be 7
	MSB(wLength) = USB_read_byte();
	USB_ack_receive_setup();	// clear the receive setup flag
	
	while (!is_USB_receive_out()); // wait for RXOUTI to be set
	baudrate = USB_read_byte();	
	baudrate |= (uint32_t)USB_read_byte() << 8;
	baudrate |= (uint32_t)USB_read_byte() << 16;
	baudrate |= (uint32_t)USB_read_byte() << 24;
	USB_read_byte();	// stop bits
	USB_read_byte();	// parity
	USB_read_byte();	// data bits
	USB_ack_receive_out();
	USB_send_control_in();
	while (!is_USB_read_control_enabled());
}

void getLineCoding(void) {
	uint16_t wValue, wIndex, wLength;
	
	LSB(wValue) = USB_read_byte();	// is 0
	MSB(wValue) = USB_read_byte();
	LSB(wIndex) = USB_read_byte();	// read interface
	MSB(wIndex) = USB_read_byte();
	LSB(wLength) = USB_read_byte();	// read lenght, should be 7
	MSB(wLength) = USB_read_byte();
	USB_ack_receive_setup();	// clear the receive setup flag
	while (!is_USB_read_control_enabled());
	USB_write_byte(0x80);
	USB_write_byte(0x25);
	USB_write_byte(0x0);
	USB_write_byte(0x0);
	USB_write_byte(0x0);	// stop bits
	USB_write_byte(0x0);	// parity
	USB_write_byte(0x08);	// data bits
	USB_send_control_in();

 	if (is_USB_receive_out()) {
 		USB_ack_receive_out();
 		return;
 	}
	
	while(!is_USB_receive_out());
	USB_ack_receive_out();	
}

void setControlLineState() {
	uint16_t wValue, wIndex, wLength;
	LSB(wValue) = USB_read_byte();	// control signal bitmap
	MSB(wValue) = USB_read_byte();
	LSB(wIndex) = USB_read_byte();	// read interface
	MSB(wIndex) = USB_read_byte();
	LSB(wLength) = USB_read_byte();	// read length, should be 0
	MSB(wLength) = USB_read_byte();
	USB_ack_receive_setup();	// clear the receive setup flag
	
	while (!is_USB_read_control_enabled());
	USB_send_control_in();

	if (is_USB_receive_out()) {
		USB_ack_receive_out();
		return;
	}
}

/*
 * This function is called by the standard usb read request function when
 * the usb request is no standard request but a device specific request.
 * This function returns TRUE when the request is processed. 
 * This function returns FALSE if the request is not supported. In this case, 
 * a STALL handshake will be automatically sent by the standard usb read request function.
 */
uint8_t usbCDCRequest(uint8_t requestType, uint8_t request) {
	switch(requestType) {
		case CDC_WRITE_REQUEST:	// host-to-device, class specific
			switch(request) {
				case SET_LINE_CODING:
					setLineCoding();
					return TRUE;
					break;
				case SET_CONTROL_LINE_STATE:
					setControlLineState();
					return TRUE;
					break;
				default:
					return FALSE;
			}
			break;
		case CDC_READ_REQUEST: // device-to-host, class specific
			switch(request) {
				case GET_LINE_CODING:
					getLineCoding();
					return TRUE;
					break;
				default:
					return FALSE;
			}
			break;	 
		default:
			return FALSE;	
	}
	return FALSE;
}

/*
 * This function configures the endpoints.
 */
void usbDeviceEndpointInit(uint8_t conf_nb) {
	// endpoint configuration could be changed
	// not implemented yet
}

/*
 * This function returns the size and the pointer on a user information structure.
 */
uint8_t usbDeviceGetDescriptor(uint8_t type, uint8_t string) {
	switch(type) {
		case USB_DT_STRING:
			switch (string) {
				case USB_LANGUAGE_ID:
					dataToTransfer = sizeof(usb_desc_language);
					buf = &(usb_desc_language.bLength);
					return TRUE;
					break;
				case USB_DD_iManufacturer:
					dataToTransfer = sizeof(usb_desc_string_manufacturer);
					buf = &(usb_desc_string_manufacturer.bLength);
					return TRUE;
					break;
				case USB_DD_iProduct:
					dataToTransfer = sizeof (usb_desc_string_product);
					buf = &(usb_desc_string_product.bLength);
					return TRUE;
					break;
				case USB_DD_iSerialNumber:
					dataToTransfer = sizeof (usb_desc_string_serial_number);
					buf = &(usb_desc_string_serial_number.bLength);
					return TRUE;
					break;
				default:
					return FALSE;
				}
		default:
			return FALSE;
	}
	return FALSE;
}