/*
 * Contains functions for request handling.
 * 1.4.2016, Urs Graf
 */

#include "config.h"
#include "usb_common.h"
#include "usb_descriptors.h"
#include "usb_requests.h"
#include "usb_cdc_requests.h"

uint8_t const *buf;
uint8_t dataToTransfer;
uint16_t wInterface;
uint8_t usbConfigurationNr;

static uint8_t statusEP[USB_EP_TOTAL_NUM + 1]; // endpoint status for [number of endpoints including endpoint 0]
static uint8_t bmRequestType;

//**********      Forward Declarations      **********//
void usbClearFeature(void);
void usbSetFeature(void);
void usbGetInterface(void);
void usbGetStatus(void);
void usbGetDescriptor(void);
void usbSetAddress(void);
void usbSetConfiguration(void);
void usbGetConfiguration(void);

//**********      Functions      **********//

/* 
 * This function initializes the main usb variables:
 * - endpoint status
 * - connection status
 */
void usbEnumVarInit (void) {
	uint8_t ep_num;

	for (ep_num = 0; ep_num < USB_EP_TOTAL_NUM + 1; ep_num++) {
		statusEP[ep_num] = 0;
	}
	usbConfigurationNr = 0;	// Default configuration number is 0
}


/*
 * This function reads the SETUP request sent to the default control endpoint
 * and calls the appropriate function. When exiting the function, 
 * the device is ready to manage the next request.
 */
void usbProcessRequest(void) {
	uint8_t bmRequest;

	bmRequestType = USB_read_byte();
	bmRequest = USB_read_byte();
	
	switch (bmRequest) {
	case GET_STATUS:	// request for device, interface or endpoint
		if ((0x80 <= bmRequestType) & (0x82 >= bmRequestType)) usbGetStatus();
		else usbCDCRequest(bmRequestType, bmRequest);
		break;
	case CLEAR_FEATURE:
		if (bmRequestType <= 2) usbClearFeature();
		else usbCDCRequest(bmRequestType, bmRequest);
		break;
	case SET_FEATURE:
		if (bmRequestType <= 2) usbSetFeature();
		else usbCDCRequest(bmRequestType, bmRequest);
		break;
	case SET_ADDRESS:
		if (0x00 == bmRequestType) usbSetAddress();
		else usbCDCRequest(bmRequestType, bmRequest);
		break;
	case GET_DESCRIPTOR:
		if (0x80 == bmRequestType) usbGetDescriptor();
		else usbCDCRequest(bmRequestType, bmRequest);
		break;
	case GET_CONFIGURATION:
		if (0x80 == bmRequestType) usbGetConfiguration();
		else usbCDCRequest(bmRequestType, bmRequest);
		break;
	case SET_CONFIGURATION:
		if (0x00 == bmRequestType) usbSetConfiguration();
		else usbCDCRequest(bmRequestType, bmRequest);
		break;
	case GET_INTERFACE:
		if (bmRequestType == 0x81) usbGetInterface();
		else usbCDCRequest(bmRequestType, bmRequest);
		break;
	case SET_DESCRIPTOR:
	case SET_INTERFACE:
	case SYNCH_FRAME:
	default: // unsupported request => call to device specific request
		if (usbCDCRequest(bmRequestType, bmRequest) == FALSE) {
			USB_enable_stall_handshake();
			USB_ack_receive_setup();
			return;
		}
		break;
	}
}

/*
 * This function manages the SET ADDRESS request. When complete, the device
 * will filter the requests using the new address.
 */
void usbSetAddress(void) {
	USB_configure_address(USB_read_byte());

	USB_ack_receive_setup();

	USB_send_control_in();	// send a ZLP for STATUS phase
	while(!is_USB_in_ready());	// waits for status phase done
								// before using the new address
	USB_enable_address();
}


/* 
 * This function manages the SET CONFIGURATION request. If the selected
 * configuration is valid, this function call the usbDeviceEndpointInit()
 * function that will configure the endpoints following the configuration
 * number.
 */
void usbSetConfiguration(void) {
	uint8_t num;

	num = USB_read_byte();

	if (num <= USB_DD_bNumConfigurations) {
		USB_ack_receive_setup();
		usbConfigurationNr = num;
	} else {
		// keep that order (set StallRq/clear RxSetup) or a
		// OUT request following the SETUP may be acknowledged
		USB_enable_stall_handshake();
		USB_ack_receive_setup();
		return;
	}
	USB_send_control_in();	// send a ZLP for STATUS phase
	usbDeviceEndpointInit(usbConfigurationNr);	// device specific endpoint configuration
}


/* 
 * This function manages the GET DESCRIPTOR request. The device descriptor,
 * the configuration descriptor and the device qualifier are supported. All
 * other descriptors must be supported by the usbDeviceGetDescriptor
 * function.
 * Only 1 configuration is supported.
*/
void usbGetDescriptor(void) {
	uint16_t  wLength;
	uint8_t descriptorType ;
	uint8_t stringType;
	uint8_t nofByte;
	uint8_t zlp;

	zlp = FALSE;	// no zero length packet
	stringType = USB_read_byte();	// read LSB of wValue
	descriptorType = USB_read_byte();	// read MSB of wValue

	switch (descriptorType) {
	case USB_DT_DEVICE:
		dataToTransfer = sizeof(usb_desc_device);
		buf = &(usb_desc_device.bLength);
		break;
	case USB_DT_CONFIG:
		dataToTransfer = sizeof(usb_desc_config);
		buf = &(usb_desc_config.config.bLength);
		break;
	default:
		if (usbDeviceGetDescriptor(descriptorType, stringType) == FALSE) {
			USB_enable_stall_handshake();
			USB_ack_receive_setup();
			return;
		}
		break;
	}

	USB_read_byte();	// dummy read, don't care about wIndex field
	USB_read_byte();
	LSB(wLength) = USB_read_byte();	// read wLength
	MSB(wLength) = USB_read_byte();
	USB_ack_receive_setup();	// clear the receive setup flag

	if (wLength > dataToTransfer) {
		if ((dataToTransfer % USB_DD_bMaxPacketSize0) == 0) {
			zlp = TRUE;
		} else {
			zlp = FALSE;	// no need of zero length packet
		}
	} else {
		dataToTransfer = (uint8_t)wLength;	// send only requested number of data
	}

	while ((dataToTransfer != 0) && (!is_USB_receive_out())) {
		while(!is_USB_read_control_enabled());

		nofByte=0;
		while(dataToTransfer != 0) {	// Send data until necessary
			if(nofByte++ == USB_DD_bMaxPacketSize0) {	// Check endpoint 0 size
				break;
			}
			USB_write_byte(*buf);
			buf++;
			dataToTransfer--;
		}
		USB_send_control_in();
	}

	USB_send_control_in();

	if (is_USB_receive_out()) {
		USB_ack_receive_out();
		return;	// abort from Host
	}
	if (zlp == TRUE) USB_send_control_in();
	
	while (!is_USB_receive_out());
	USB_ack_receive_out();
}


/* 
 * This function manages the GET CONFIGURATION request. The current
 * configuration number is returned.
*/
void usbGetConfiguration(void) {
	USB_ack_receive_setup();

	USB_write_byte(usbConfigurationNr);
	USB_ack_in_ready();

	while(!is_USB_receive_out());
	USB_ack_receive_out();
}

/*
 * This function manages the GET STATUS request. The device, interface or
 * endpoint status is returned.
*/
void usbGetStatus(void) {
	uint8_t wIndex;

	USB_read_byte();				// dummy reads for wValue
	USB_read_byte();
	LSB(wIndex) = USB_read_byte();	
	MSB(wIndex) = USB_read_byte();	// forget about wlength

	switch(bmRequestType) {
	case REQ_STATUS_DEVICE:
		USB_ack_receive_setup();	
		USB_write_byte(STATUS_DEVICE);	// LSB
		break;
	case REQ_STATUS_INTERFACE:
		USB_ack_receive_setup();
		USB_write_byte(STATUS_INTERFACE);	// LSB
		break;
	case REQ_STATUS_ENDPOINT:
		USB_ack_receive_setup();
		wIndex = wIndex & EP_MASK_DIR;
		USB_write_byte(statusEP[wIndex]);	// LSB
		break;
	default:
		USB_enable_stall_handshake();
		USB_ack_receive_setup();
		return;
	}

	USB_write_byte(0x00);	// MSB of statux information is always 0
	USB_send_control_in();

	while(!is_USB_receive_out());
	USB_ack_receive_out();
}

/* 
 * This function manages the SET FEATURE request. The USB test modes are
 * supported by this function.
*/
void usbSetFeature(void) {
	uint8_t wValue;
	uint8_t wIndex;

	if (bmRequestType == USB_REQ_TYPE_INTERFACE) {
		// keep that order (set StallRq/clear RxSetup) or a
		// OUT request following the SETUP may be acknowledged
		USB_enable_stall_handshake();
		USB_ack_receive_setup();
		return;
	} else if (bmRequestType == USB_REQ_TYPE_ENDPOINT) {
		wValue = USB_read_byte();
		USB_read_byte();	// dummy read

		if (wValue == USB_FEATURE_EP_HALT) {
			wIndex = (USB_read_byte() & EP_MASK_DIR);
	
			if (wIndex == EP_CONTROL) {
				USB_enable_stall_handshake();
				USB_ack_receive_setup();
				return;
			}
	
			USB_select_endpoint(wIndex);
			if(is_USB_endpoint_enabled()) {
				USB_enable_stall_handshake();
				USB_select_endpoint(EP_CONTROL);
				statusEP[wIndex] = 0x01;
				USB_ack_receive_setup();
				USB_send_control_in();
			} else {
				USB_enable_stall_handshake();
				USB_ack_receive_setup();
				return;
			}
		} else {
			USB_enable_stall_handshake();
			USB_ack_receive_setup();
			return;
		}
	}
}


/* 
 * This function manages the CLEAR FEATURE request.
*/
void usbClearFeature(void) {
	uint8_t wValue;
	uint8_t wIndex;

	if (bmRequestType == USB_REQ_TYPE_ZERO) {
		// keep that order (set StallRq/clear RxSetup) or a
		// OUT request following the SETUP may be acknowledged
		USB_enable_stall_handshake();
		USB_ack_receive_setup();
		return;
	} else if (bmRequestType == USB_REQ_TYPE_INTERFACE) {
		// keep that order (set StallRq/clear RxSetup) or a
		// OUT request following the SETUP may be acknowledged
		USB_enable_stall_handshake();
		USB_ack_receive_setup();
		return;
	} else if (bmRequestType == USB_REQ_TYPE_ENDPOINT) {
		wValue = USB_read_byte();
		USB_read_byte();	// dummy read

		if (wValue == USB_FEATURE_EP_HALT) {
			wIndex = (USB_read_byte() & EP_MASK_DIR);

			USB_select_endpoint(wIndex);
			if(is_USB_endpoint_enabled()) {
				if(wIndex != EP_CONTROL) {
					USB_disable_stall_handshake();
					USB_reset_endpoint(wIndex);
					USB_reset_data_toggle();
				}
				USB_select_endpoint(EP_CONTROL);
				statusEP[wIndex] = 0x00;
				USB_ack_receive_setup();
				USB_send_control_in();
			} else {
				USB_enable_stall_handshake();
				USB_ack_receive_setup();
				return;
			}
		} else {
			USB_enable_stall_handshake();
			USB_ack_receive_setup();
			return;
		}
	}
}

/*
 * This function manages the GET_INTERFACE request.
 */
void usbGetInterface(void) {
	USB_read_byte();	// dummy read
	USB_read_byte();
	LSB(wInterface) = USB_read_byte();	// read wInterface
	MSB(wInterface) = USB_read_byte();

	USB_enable_stall_handshake();
	USB_ack_receive_setup();
}
