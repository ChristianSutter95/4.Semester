#ifndef USB_REQUESTS_H_
#define USB_REQUESTS_H_

/*
 * Definitions for USB standard requests.
 * 1.4.2015, Urs Graf
 */

//**********      Standard requests      **********//
#define GET_STATUS                     0x00
#define GET_DEVICE                     0x01
#define CLEAR_FEATURE                  0x01           //!< see FEATURES below
#define GET_STRING                     0x03
#define SET_FEATURE                    0x03           //!< see FEATURES below
#define CLEAR_COMM_FEATURE			   0x04
#define SET_ADDRESS                    0x05
#define GET_DESCRIPTOR                 0x06
#define SET_DESCRIPTOR                 0x07
#define GET_CONFIGURATION              0x08
#define SET_CONFIGURATION              0x09
#define GET_INTERFACE                  0x0A
#define SET_INTERFACE                  0x0B
#define SYNCH_FRAME                    0x0C

#define GET_DEVICE_DESCRIPTOR             1
#define GET_CONFIGURATION_DESCRIPTOR      4

//**********      GET_STATUS request types      **********//
#define REQ_STATUS_DEVICE					0x80
#define REQ_STATUS_INTERFACE				0x81
#define REQ_STATUS_ENDPOINT					0x82

//**********      Standard request types      **********//
#define USB_REQ_TYPE_ZERO					0x00
#define USB_REQ_TYPE_INTERFACE				0x01
#define USB_REQ_TYPE_ENDPOINT				0x02

//**********      Standard features      **********//
#define USB_FEATURE_DEVICE_REMOTE_WAKEUP	0x01
#define USB_FEATURE_EP_HALT					0x00

//**********      Functions      **********//
void usbProcessRequest(void);

//**********      Variables      **********//
extern uint8_t const *buf;
extern uint8_t dataToTransfer;
extern uint16_t wInterface;
extern uint8_t usbConfigurationNr;

#endif /*USB_REQUESTS_H_*/
