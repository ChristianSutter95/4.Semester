/*
 * Contains functions to initialize the USB. 
 * Handles USB device interrupts and endpoint interrupts.
 * Sends state of dip switch to host (once per 1000 requests)
 * Receives state for led from host
 * 1.4.2016, Urs Graf
 */

#include "config.h"
#include "pll.h"
#include "usb_common.h"
#include "usb_requests.h"

uint16_t count;

//**********      Functions      **********//

uint8_t usbConfigEp(uint8_t config0, uint8_t config1) {
	USB_enable_endpoint();
    UECFG0X = config0;
    UECFG1X = (UECFG1X & (1<<ALLOC)) | config1;
    USB_allocate_memory();
    return (is_endpoint_configured());
}

void usbCommonInit(void) {
	USB_force_device_mode();
	USB_enable();
	USB_select_device();	
	USB_enable_vbus_interrupt();
}

//**********      USB Endpoint Interrupt Handling      **********//

uint8_t getEndpointNrFromIrq() {
	uint8_t i = 0;
	for (i = 0; i < 8; i++) {
		if ((UEINT & (1<<i)) != 0) return i;
	}	
	return 0;
}

void handleEPControlIRQ() {			// endpoint 0, control 
	if (is_USB_receive_setup()) {	// a setup packet was received
		usbProcessRequest();		// handle the request therein
		USB_ack_receive_setup();
	}
}

void handleEP1IRQ() {				// endpoint 1, interrupt
	// the only request hitting this endpoint is SERIAL_STATE
	// which is defined by the CDC 
	USB_write_byte(0x00);			// send serial state information
	USB_write_byte(0x00);			// the only request hitting this endpoint
	USB_send_control_in();
	USB_ack_fifocon();
}

void handleEP2IRQ() {				// endpoint 2, bulk in
	count++;
	if (count == 1000) {
		count = 0;
		if (PIND & 0x80) USB_write_byte('a'); else USB_write_byte('b');
	}
	USB_send_control_in();
	USB_ack_fifocon();
}

void handleEP3IRQ() {				// endpoint 3, bulk out
	uint8_t val;
	if(is_USB_receive_out()) {
		USB_ack_control_out();
		val = USB_read_byte();
		if (val == 'x') PORTB |= 1; else PORTB &= ~1;
		USB_ack_receive_out();
	}
}

ISR(USB_COM_vect) {
	uint8_t ep = 0;
	
	while(UEINT != 0) {
		ep = getEndpointNrFromIrq();
		USB_select_endpoint(ep);
		switch(ep){
			case EP_CONTROL:
				handleEPControlIRQ();
				break;
			case EP_1:	
				handleEP1IRQ();
				break;
			case EP_2:
				handleEP2IRQ();
				break;
			case EP_3:
				handleEP3IRQ();
				break;
			default : 
				UEINTX = 0;	// default endpoint
				break;
		}
	}	
}

//**********      USB General Interrupt Handling      **********//

void handleVbusTransitionIRQ() {
	if (is_USB_vbus_high()) {
		USB_enable_pad_regulator();
		PLL_start_auto();
		wait_PLL_ready();
		USB_enable_reset_interrupt();
		USB_unfreeze_clock();
		USB_attach();
	} else {
		USB_detach();
	}
	USB_ack_vbus_transition();
}

void handleResetIRQ() {
	usbConfigurationNr = 0;
	// Endpoint Control	
	USB_select_endpoint(EP_CONTROL);
	USB_enable_endpoint();
	USB_configure_endpoint(EP_CONTROL, USB_EP_TYPE_CONTROL, USB_EP_DIR_OUT, EP_SIZE_64, EP_ONE_BANK, EP_NYET_DISABLED);
	USB_enable_receive_setup_interrupt();	
	// Endpoint 1
	USB_select_endpoint(USB_EP1_NUM);
	USB_enable_endpoint();
	USB_configure_endpoint(USB_EP1_NUM,	USB_EP1_TYPE, USB_EP1_DIR, USB_EP1_SIZE, EP_ONE_BANK, EP_NYET_ENABLED);
	USB_enable_in_ready_interrupt();
	// Endpoint 2
	USB_select_endpoint(USB_EP2_NUM);
	USB_enable_endpoint();
	USB_configure_endpoint(USB_EP2_NUM,	USB_EP2_TYPE, USB_EP2_DIR, USB_EP2_SIZE, EP_ONE_BANK, EP_NYET_ENABLED);
	USB_enable_in_ready_interrupt();
	// Endpoint 3
	USB_select_endpoint(USB_EP3_NUM);
	USB_enable_endpoint();
	USB_configure_endpoint(USB_EP3_NUM,	USB_EP3_TYPE, USB_EP3_DIR, USB_EP3_SIZE, EP_ONE_BANK, EP_NYET_ENABLED);
	USB_enable_receive_out_interrupt();
	// Reset all Endpoints	
	USB_reset_endpoint(EP_CONTROL);
	USB_reset_endpoint(USB_EP1_NUM);
	USB_reset_endpoint(USB_EP2_NUM);
	USB_reset_endpoint(USB_EP3_NUM);
		
	USB_ack_reset();
}

void handleIDTransitionIRQ() {
	// used for detection of another host 
	USB_ack_id_transition();
	// no action taken
}	

void handleSOFIRQ() {
	USB_ack_sof();
	// no action taken
}

void handleSusplendIRQ() {
	//USB_enable_wake_up_interrupt();
	USB_ack_wake_up();                 // clear wake up to detect next event
	USB_freeze_clock();
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_mode();
	USB_ack_suspend();
}	

void handleUSBWakeUpIRQ() {
	//USB_unfreeze_clock();
	USB_ack_wake_up();
	USB_disable_wake_up_interrupt();
	// no action taken
}	

void handleUSBResumeIRQ() {
	USB_disable_wake_up_interrupt();
	USB_disable_resume_interrupt();
	// no action taken
	USB_ack_resume();
}	
	

ISR (USB_GEN_vect) {		
	if (is_USB_vbus_transition()) handleVbusTransitionIRQ();
	if (is_USB_reset())	handleResetIRQ();
	if (is_USB_id_transition()) handleIDTransitionIRQ();
	if (is_USB_sof()) handleSOFIRQ();
	if (is_USB_suspend()) handleSusplendIRQ();
	if (is_USB_wake_up()) handleUSBWakeUpIRQ();
	if (is_USB_resume()) handleUSBResumeIRQ();
}	
	