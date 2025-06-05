#ifndef USB_DESCRIPTORS_H_
#define USB_DESCRIPTORS_H_

/*
 * Definitions for USB descriptors.
 * 15.4.2015, Urs Graf
 */

#include "typedefs.h"

//**********      USB descriptor types      **********//

#define USB_DT_DEVICE				0x01
#define USB_DT_CONFIG				0x02
#define USB_DT_STRING				0x03
#define USB_DT_INTERFACE			0x04
#define USB_DT_ENDPOINT				0x05
#define USB_DT_DEVQUAL				0x06
#define USB_DT_OTHER_SPEED_CONFIG	0x07

#define USB_DT_HID					0x21
#define USB_DT_REPORT				0x22
#define USB_DT_PHYSICAL				0x23
#define USB_DT_CS_INTERFACE			0x24
#define USB_DT_HUB					0x29

//**********      USB sub types      **********//
#define USB_ST_HEADER				0x00
#define USB_ST_CALL_MANG			0x01
#define USB_ST_ABSTR_CTR			0x02
#define USB_ST_UNION				0x06

//**********      USB configuration defines      **********//
#define USB_CONF_ATTRIBUTES_RESERVED	0x80
#define USB_CONF_BUSPOWERED				(USB_CONF_ATTRIBUTES_RESERVED | 0x00)
#define USB_CONF_SELFPOWERED			(USB_CONF_ATTRIBUTES_RESERVED | 0x40)
#define USB_CONF_REMOTEWAKEUP			(USB_CONF_ATTRIBUTES_RESERVED | 0x20)

//**********      USB endpoint defines      **********//

// Endpoint types
#define USB_EP_TYPE_CONTROL			0x00	// bmAttributes
#define USB_EP_TYPE_ISOCHRONOUS		0x01
#define USB_EP_TYPE_BULK			0x02
#define USB_EP_TYPE_INTERRUPT		0x03

// Endpoint directions
#define USB_EP_DIR_OUT				0		// bEndpointAddress
#define USB_EP_DIR_IN				1
#define USB_EP_DIR_INbm				0x80

// Masks
#define USB_EP_MASK_TYPE			0x03	// bmAttributes
#define USB_EP_MASK_ADDR			0x0F	// bEndpointAddress
#define USB_EP_MASK_DIR				0x80	// bEndpointAddress

//**********      USB string descriptor defines      **********//

#define USB_unicode(v)				((uint16_t)(v))
#define USB_LANGUAGE_ID				0x00	// string type value
#define USB_LANGUAGE_ID_US			0x0409	// magic code for US English


//**********      USB descriptor typedefs      **********//

// USB device descriptor
typedef struct {
	uint8_t bLength;				// Size of descriptor in bytes
	uint8_t bDescriptorType;		// Type of descriptor (USB_DT_DEVICE)
	uint16_t bcdUSB;				// USB Specification number
	uint8_t bDeviceClass;			// Class code
	uint8_t bDeviceSubClass;		// Subclass code
	uint8_t bDeviceProtocol;		// Protocol code
	uint8_t bMaxPacketSize0;		// Maximum packet size for endpoint zero
	uint16_t idVendor;				// Vendor ID
	uint16_t idProduct;				// Product ID
	uint16_t bcdDevice;				// Device release number
	uint8_t iManufacturer;			// Index of manufacturer string descriptor
	uint8_t iProduct;				// Index of product string descriptor
	uint8_t iSerialNumber;			// Index of serial number string descriptor
	uint8_t bNumConfigurations;		// Number of possible configurations
} usb_desc_device_type;

// USB configuration descriptor
typedef struct {
	uint8_t bLength;				// Size of descriptor in bytes
	uint8_t bDescriptorType;		// Type of descriptor (USB_DT_CONFIG)
	uint16_t wTotalLength;			// Total length in bytes of data returned
	uint8_t bNumInterfaces;			// Number of interfaces
	uint8_t bConfigurationValue;	// Value (identifier) of this configuration
	uint8_t iConfiguration;			// Index of String descriptor describing this configuration
	uint8_t bmAttributes;			// Specifies power parameters for this configuration
	uint8_t MaxPower;				// Maximum power consumption in 2mA units
} usb_desc_config_type;

// USB interface descriptor
typedef struct {
	uint8_t bLength;				// Size of descriptor in bytes
	uint8_t bDescriptorType;		// Type of descriptor (USB_DT_INTERFACE)
	uint8_t bInterfaceNumber;		// Number (identifier) of this interface
	uint8_t bAlternateSetting;		// Value used to select alternate setting
	uint8_t bNumEndpoints;			// Number of Endpoints used for this interface
	uint8_t bInterfaceClass;		// Class code
	uint8_t bInterfaceSubClass;		// Subclass code
	uint8_t bInterfaceProtocol;		// Protocol code
	uint8_t iInterface;				// Index of String descriptor describing this interface
} usb_desc_interface_type;

// USB endpoint descriptor
typedef struct {
	uint8_t bLength;				// Size of descriptor in bytes
	uint8_t bDescriptorType;		// Type of descriptor (USB_DT_ENDPOINT)
	uint8_t bEndpointAddress;		// Endpoint Address
	uint8_t bmAttributes;			// Bits 1..0: Transfer Type, Bits 7..2: Reserved
	uint16_t wMaxPacketSize;		// Maximum packet size
	uint8_t bInterval;				// Interval for polling endpoint data transfers
} usb_desc_endpoint_type;

// USB language descriptor
typedef struct {
	uint8_t bLength;				// Size of descriptor in bytes
	uint8_t bDescriptorType;		// Type of descriptor (USB_DT_STRING)
	uint16_t wlangid;				// Language id
} usb_desc_language_type;

// USB HID descriptor
typedef struct {
	uint8_t  bLength;				// Size of descriptor in bytes
	uint8_t  bDescriptorType;		// Type of descriptor (USB_DT_HID)
	uint16_t bscHID;				// HID specification number
	uint8_t  bCountryCode;			// Country code
	uint8_t  bNumDescriptors;		// Number of descriptors
	uint8_t  bRDescriptorType;		// Report descriptor type
	uint16_t wDescriptorLength;		// Total length of report descriptor
} usb_desc_hid_type;

// USB Header descriptor
typedef struct {
	uint8_t  bLength;				// Size of descriptor in bytes
	uint8_t  bDescriptorType;		// CS_INTERFACE descriptor type
	uint8_t  bDescriptorSubtype;	// Header functional descriptor subtype				
	uint16_t bcdCDC;				// USB Class Definitions for Communication Devices Specification release number in binary-coded decimal.
} usb_header_desc_hid_type;

// USB Abstract Control Model descriptor
typedef struct {
	uint8_t  bLength;				// Size of descriptor in bytes
	uint8_t  bDescriptorType;		// CS_INTERFACE descriptor type
	uint8_t  bDescriptorSubtype;	// Header functional descriptor subtype				
	uint8_t  bmCapabilities;		// The capabilities that this configuration supports
} usb_abstract_ctr_model_desc_hid_type;

// USB Union descriptor
typedef struct {
	uint8_t  bLength;				// Size of descriptor in bytes
	uint8_t  bDescriptorType;		// CS_INTERFACE descriptor type
	uint8_t  bDescriptorSubtype;	// Union functional descriptor SubType				
	uint8_t  bMasterInterface;		// The interface number of the Communication or Data Class interface, designated as the master or controlling interface for the union.
	uint8_t  bSlaveInterface0;		// Interface number of first slave or associated interface in the union.
} usb_union_desc_hid_type;

// USB Call Management descriptor
typedef struct {
	uint8_t  bLength;				// Size of descriptor in bytes
	uint8_t  bDescriptorType;		// CS_INTERFACE descriptor type
	uint8_t  bDescriptorSubtype;	// Union functional descriptor SubType				
	uint8_t  bmCapabilities;		// The capabilities that this configuration supports.
	uint8_t  bDataInterface;		// Interface number of Data Class interface optionally used for call management.
} usb_call_management_desc_hid_type;

#endif /*USB_DESCRIPTORS_H_*/
