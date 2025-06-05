#ifndef CONFIG_H_
#define CONFIG_H_

/*
 * Contains complete USB descriptors (device / configuration, interface, endpoint).
 * Configures the AVR to work as a CDC device with custom vendor and product id
 * 6.4.2016, Urs Graf
 */

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include "typedefs.h"
#include "usb_descriptors.h"

//**********      USB device descriptor values      **********//

#define USB_DD_bcdUSB				0x0200		// USB specification number
#define USB_DD_bDeviceClass			0x02		// Device class (vendor specific)
#define USB_DD_bDeviceSubClass		0x02		// Device subclass (vendor specific)
#define USB_DD_bDeviceProtocol		0x01		// Device protocol (vendor specific)
#define USB_DD_bMaxPacketSize0		64			// Maximum packet size for endpoint zero
#define USB_DD_idVendor				0x03EB		// Vendor ID (NTB)
#define USB_DD_idProduct			0xAFFE		// Product ID (inf SEA-board)
#define USB_DD_bcdDevice			0x0600		// Device release number
#define USB_DD_iManufacturer		0x01		// Index of manufacturer string descriptor
#define USB_DD_iProduct				0x02		// Index of product string descriptor
#define USB_DD_iSerialNumber		0x03		// Index of serial number string descriptor
#define USB_DD_bNumConfigurations	1			// Number of possible configurations

//**********      Configuration definitions      **********//

#define USB_CONF1_bNumInterfaces		1		// Number of interfaces
#define USB_CONF1_bConfigurationValue	1		// Index of this configuration
#define USB_CONF1_iConfiguration		0		// Index of String descriptor
#define USB_CONF1_bmAttributes			USB_CONF_BUSPOWERED
#define USB_CONF1_MaxPower				50		// Maximum power consumption in 2mA steps => 100mA

//**********      Interface definitions      **********//

#define USB_INT1_bInteraceNumber		0		// Number (identifier) of interface
#define USB_INT1_bAlternateSetting		0		// Value to select alternate setting
#define USB_INT1_bNumEndpoints			3		// Number of endpoints
#define USB_INT1_bInterfaceClass		0x2	// Interface class
#define USB_INT1_bInterfaceSubClass		0x2	// Interface sub class
#define USB_INT1_bInterfaceProtocol		0x1	// Interface protocol
#define USB_INT1_iInterface				0		// Index of String descriptor

//**********      Endpoint definitions      **********//

#define USB_EP_TOTAL_NUM				3		// Total number of endpoints (excluding ep zero)
// Device values
#define USB_EP1_NUM						EP_1					// Endpoint number (identifier)
#define USB_EP1_TYPE					USB_EP_TYPE_INTERRUPT	// Endpoint Type
#define USB_EP1_DIR						USB_EP_DIR_IN
#define USB_EP1_SIZE					EP_SIZE_8
// Descriptor values
#define USB_EP1_bEndpointAddress		(USB_EP1_NUM | USB_EP_DIR_INbm)	// EP1 is input
#define USB_EP1_bmAttributes			USB_EP1_TYPE			// Interrupt
#define USB_EP1_wMaxPacketSize			16			// Maximum packet size
#define USB_EP1_bInterval				0x02					// Interval (every second frame)

// Device values
#define USB_EP2_NUM						EP_2					// Endpoint number (identifier)
#define USB_EP2_TYPE					USB_EP_TYPE_BULK		// Endpoint Type
#define USB_EP2_DIR						USB_EP_DIR_IN
#define USB_EP2_SIZE					EP_SIZE_64
// Descriptor values
#define USB_EP2_bEndpointAddress		(USB_EP2_NUM | USB_EP_DIR_INbm)	// EP2 is input
#define USB_EP2_bmAttributes			USB_EP2_TYPE			// bulk
#define USB_EP2_wMaxPacketSize			64			// Maximum packet size
#define USB_EP2_bInterval				0x00					// No interval

// Device values
#define USB_EP3_NUM						EP_3					// Endpoint number (identifier)
#define USB_EP3_TYPE					USB_EP_TYPE_BULK		// Endpoint Type
#define USB_EP3_DIR						USB_EP_DIR_OUT
#define USB_EP3_SIZE					EP_SIZE_64
// Descriptor values
#define USB_EP3_bEndpointAddress		USB_EP3_NUM				// EP3 is output
#define USB_EP3_bmAttributes			USB_EP3_TYPE			// bulk
#define USB_EP3_wMaxPacketSize			64			// Maximum packet size
#define USB_EP3_bInterval				0x00					// No interval

//**********      Get Status definitions       **********//
// USB Spec 9.4.5 (Get Status)
#define STATUS_DEVICE_SELF_POWERED		0x01
#define STATUS_DEVICE_REMOTE_WAKEUP		0x02
#define STATUS_INTERFACE_DEFAULT		0x00

// STATUS_DEVICE must correspond to USB_CONFx_bmAttributes
#define STATUS_DEVICE		STATUS_DEVICE_SELF_POWERED
#define STATUS_INTERFACE	STATUS_INTERFACE_DEFAULT

//**********      USB string descriptor values      **********//

#define USB_SD_MANUFACTURER_NAME_LENGTH		10
#define USB_SD_MANUFACTURER_NAME \
{ USB_unicode('i') \
, USB_unicode('n') \
, USB_unicode('f') \
, USB_unicode('.') \
, USB_unicode('n') \
, USB_unicode('t') \
, USB_unicode('b') \
, USB_unicode('.') \
, USB_unicode('c') \
, USB_unicode('h') \
}

#define USB_SD_PRODUCT_NAME_LENGTH		12
#define USB_SD_PRODUCT_NAME \
{ USB_unicode('A') \
, USB_unicode('V') \
, USB_unicode('R') \
, USB_unicode('-') \
, USB_unicode('C') \
, USB_unicode('O') \
, USB_unicode('M') \
, USB_unicode(' ') \
, USB_unicode('P') \
, USB_unicode('o') \
, USB_unicode('r') \
, USB_unicode('t') \
}

#define USB_SD_SERIAL_NUMBER_LENGTH		8
#define USB_SD_SERIAL_NUMBER \
{ USB_unicode('0') \
, USB_unicode('0') \
, USB_unicode('.') \
, USB_unicode('0') \
, USB_unicode('1') \
, USB_unicode('.') \
, USB_unicode('0') \
, USB_unicode('0') \
}

//**********      USB string descriptor typedefs      **********//

/* This typedefs can not be included in usb_descriptor.h, as the string lengths
 * are defined in config.h -> would result in a cyclic import
 */

// Manufacturer string
typedef struct {
   uint8_t  bLength;					// Size of descriptor in bytes
   uint8_t  bDescriptorType;			// Type of descriptor
   uint16_t wstring[USB_SD_MANUFACTURER_NAME_LENGTH];	// unicode characters
} usb_desc_string_manufacturer_type;

// Product string
typedef struct {
   uint8_t  bLength;					// Size of descriptor in bytes
   uint8_t  bDescriptorType;			// Type of descriptor
   uint16_t wstring[USB_SD_PRODUCT_NAME_LENGTH];	// unicode characters
} usb_desc_string_product_type;

// Serial number string
typedef struct {
   uint8_t  bLength;					// Size of descriptor in bytes
   uint8_t  bDescriptorType;			// Type of descriptor
   uint16_t wstring[USB_SD_SERIAL_NUMBER_LENGTH];	// unicode characters
} usb_desc_string_serial_number_type;

//**********      Device specific configuration descriptor      **********//

typedef struct {
	usb_desc_config_type config;
	usb_desc_interface_type comInterface;
	usb_desc_endpoint_type ep1;
	usb_desc_endpoint_type ep2;
	usb_desc_endpoint_type ep3;
} usb_desc_config_device_type;

// USB descriptors
extern const usb_desc_device_type usb_desc_device;
extern const usb_desc_config_device_type usb_desc_config;
extern const usb_desc_string_manufacturer_type usb_desc_string_manufacturer;
extern const usb_desc_string_product_type usb_desc_string_product;
extern const usb_desc_string_serial_number_type usb_desc_string_serial_number;
extern const usb_desc_language_type usb_desc_language;

#endif /*CONFIG_H_*/
