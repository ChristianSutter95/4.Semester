/*
 * Contains a complete set of descriptors (device / configuration / interface / endpoint).
 * 1.3.2016, Urs Graf
 */

#include "config.h"
#include "usb_descriptors.h"
#include "usb_common.h"
#include "usb_requests.h"


// Device descriptor
const usb_desc_device_type usb_desc_device = {
	sizeof(usb_desc_device_type)
	, USB_DT_DEVICE
	, USB_DD_bcdUSB
	, USB_DD_bDeviceClass
	, USB_DD_bDeviceSubClass
	, USB_DD_bDeviceProtocol
	, USB_DD_bMaxPacketSize0
	, USB_DD_idVendor
	, USB_DD_idProduct
	, USB_DD_bcdDevice
	, USB_DD_iManufacturer
	, USB_DD_iProduct
	, USB_DD_iSerialNumber
	, USB_DD_bNumConfigurations
};

// Configuration descriptor
const usb_desc_config_device_type usb_desc_config = {
	{
		sizeof(usb_desc_config_type)
		, USB_DT_CONFIG
		, sizeof(usb_desc_config_type)
				+ sizeof(usb_desc_interface_type) 
				+ sizeof(usb_desc_endpoint_type)
				+ sizeof(usb_desc_endpoint_type)
				+ sizeof(usb_desc_endpoint_type)
		, USB_CONF1_bNumInterfaces
		, USB_CONF1_bConfigurationValue
		, USB_CONF1_iConfiguration
		, USB_CONF1_bmAttributes
		, USB_CONF1_MaxPower
	}
	,
	{ // interface
		sizeof(usb_desc_interface_type)
		, USB_DT_INTERFACE
		, USB_INT1_bInteraceNumber
		, USB_INT1_bAlternateSetting
		, USB_INT1_bNumEndpoints
		, USB_INT1_bInterfaceClass
		, USB_INT1_bInterfaceSubClass
		, USB_INT1_bInterfaceProtocol
		, USB_INT1_iInterface
	}
	,
	{ // endpoint 1 (interrupt in)
		sizeof(usb_desc_endpoint_type)
		, USB_DT_ENDPOINT
		, USB_EP1_bEndpointAddress
		, USB_EP1_bmAttributes
		, USB_EP1_wMaxPacketSize
		, USB_EP1_bInterval
	}
	,
	{ // endpoint 2 (bulk in)
		sizeof(usb_desc_endpoint_type)
		, USB_DT_ENDPOINT
		, USB_EP2_bEndpointAddress
		, USB_EP2_bmAttributes
		, USB_EP2_wMaxPacketSize
		, USB_EP2_bInterval
	}
	,
	{ // endpoint 3 (bulk out)
		sizeof(usb_desc_endpoint_type)
		, USB_DT_ENDPOINT
		, USB_EP3_bEndpointAddress
		, USB_EP3_bmAttributes
		, USB_EP3_wMaxPacketSize
		, USB_EP3_bInterval
	}
};

const usb_desc_string_manufacturer_type usb_desc_string_manufacturer = {
	sizeof(usb_desc_string_manufacturer_type)
	, USB_DT_STRING
	, USB_SD_MANUFACTURER_NAME
};

const usb_desc_string_product_type usb_desc_string_product = {
	sizeof(usb_desc_string_product_type)
	, USB_DT_STRING
	, USB_SD_PRODUCT_NAME
};

const usb_desc_string_serial_number_type usb_desc_string_serial_number = {
	sizeof(usb_desc_string_serial_number_type)
	, USB_DT_STRING
	, USB_SD_SERIAL_NUMBER
};

const usb_desc_language_type usb_desc_language = {
	sizeof(usb_desc_language_type)
	, USB_DT_STRING
	, USB_LANGUAGE_ID_US
};

