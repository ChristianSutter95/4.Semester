#ifndef USB_COMMON_H_
#define USB_COMMON_H_

/*
 * General definitions for the USB, device and host mode.
 * 22.3.2017, Urs Graf
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "typedefs.h"

//**********      Endpoint definitions      **********//
#define EP_CONTROL				0
#define EP_1					1
#define EP_2					2
#define EP_3					3
#define EP_4					4
#define EP_5					5
#define EP_6					6
#define EP_7					7

#define EP_SIZE_8				0
#define EP_SIZE_16				1
#define EP_SIZE_32				2
#define EP_SIZE_64				3
#define EP_SIZE_128				4
#define EP_SIZE_256				5
#define EP_SIZE_512				6
#define EP_SIZE_1024			7

#define EP_ONE_BANK				0
#define EP_TWO_BANKS			1

#define EP_NYET_ENABLED			0
#define EP_NYET_DISABLED		1

#define EP_MASK_DIR				0x7F
#define EP_MASK_UADD			0x7F
#define EP_MASK_EPTYPE			0xC0
#define EP_MASK_EPSIZE			0x70
#define EP_MASK_EPBK			0x0C
#define EP_MASK_DTSEQ			0x0C
#define EP_MASK_NBUSYBK			0x03
#define EP_MASK_CURRBK			0x03
#define EP_MASK_DAT				0xFF  // UEDATX
#define EP_MASK_BYCTH			0x07  // UEBCHX
#define EP_MASK_BYCTL			0xFF  // UEBCLX
#define EP_MASK_EPINT			0x7F  // UEINT
#define EP_MASK_HADDR			0xFF  // UHADDR

//**********      General USB management      **********//
#define USB_enable_pad_regulator()		(UHWCON |= (1 << UVREGE))
#define USB_disable_pad_regulator()		(UHWCON &= ~(1 << UVREGE))
#define USB_enable_uid_pin()			(UHWCON |= (1 << UIDE))
#define USB_disable_uid_pin()			(UHWCON &= ~(1 << UIDE))

#if defined(__AVR_ATmega16U4__) || defined(__AVR_ATmega32U4__)	// no host mode available
	#define USB_force_device_mode()		
	#define USB_select_device()				
	#define USB_ack_id_transition()	
	#define is_USB_id_transition()		(FALSE)
#elif defined(__AVR_AT90USB1287__)
	#define USB_force_device_mode()		(USB_disable_uid_pin(), UHWCON |= (1 << UIMOD))
	#define USB_select_device()			(USBCON &= ~(1 << HOST))
	#define USB_ack_id_transition()		(USBINT = ~(1 << IDTI))
	#define is_USB_id_transition()		((USBINT & (1 << IDTI)) ? TRUE : FALSE)
#else
	#error (no supported host mode settings for this processor type)
#endif

#define USB_force_host_mode()			(USB_disable_uid_pin(), UHWCON &= ~(1 << UIMOD))
#define USB_enable_uvcon_pin()			(UHWCON |= (1 << UVCONE))
#define USB_full_speed_mode()           (UHWCON |= (1<<UDSS))
#define USB_low_speed_mode()            (UHWCON &= ~(1<<UDSS))
#define USB_enable()					(USBCON |= ((1 << USBE) | (1 << OTGPADE)))
#define USB_disable()					(USBCON &= ~((1 << USBE) | (1 << OTGPADE)))
#define USB_select_host()				(USBCON |= (1 << HOST))
#define USB_freeze_clock()				(USBCON |= (1 << FRZCLK))
#define USB_unfreeze_clock()			(USBCON &= ~(1 << FRZCLK))
#define USB_enable_id_interrupt()		(USBCON |= (1 << IDTE))
#define USB_disable_id_interrupt()		(USBCON &= ~(1 << IDTE))
#define USB_enable_vbus_interrupt()		(USBCON |= (1 << VBUSTE))
#define USB_disable_vbus_interrupt()	(USBCON &= ~(1 << VBUSTE))
#define is_USB_enabled()				((USBCON & (1 << USBE)) ? TRUE : FALSE)
#define is_USB_host_enabled()			((USBCON & (1 << HOST)) ? TRUE : FALSE)
#define is_USB_clock_freezed()			((USBCON & (1 << FRZCLK)) ? TRUE : FALSE)
#define is_USB_vbus_high()				((USBSTA & (1 << VBUS)) ? TRUE : FALSE)
#define is_USB_high_speed()				((USBSTA & (1 << SPEED)) ? TRUE : FALSE)
#define is_USB_id_device()				((USBSTA & (1 << ID)) ? TRUE : FALSE)
#define USB_ack_vbus_transition()		(USBINT = ~(1 << VBUSTI))
#define is_USB_vbus_transition()		((USBINT & (1 << VBUSTI)) ? TRUE : FALSE)
#define USB_enter_dpram_mode()			(UDPADDH = (1 << DPACC))
#define USB_exit_dpram_mode()			(UDPADDH = (uint8_t)~(1 << DPACC))
#define USB_set_dpram_address(addr)		(UDPADDH = (1 << DPACC) + ((Uint16)addr >> 8), UDPADDL = (Uchar)addr)
#define USB_write_dpram_byte(val)		(UEDATX = val)
#define USB_read_dpram_byte()			(UEDATX)
#define USB_enable_vbus()				(OTGCON |= (1 << VBUSREQ))
#define USB_disable_vbus()				(OTGCON &= ~(1 << VBUSREQ))
#define USB_device_initiate_hnp()		(OTGCON |= (1 << HNPREQ))
#define USB_host_accept_hnp()			(OTGCON |= (1 << HNPREQ))
#define USB_host_reject_hnp()			(OTGCON &= ~(1 << HNPREQ))
#define USB_device_initiate_srp()		(OTGCON |= (1 << SRPREQ))
#define USB_select_vbus_srp_method()	(OTGCON |= (1 << SRPSEL))
#define USB_select_data_srp_method()	(OTGCON &= ~(1 << SRPSEL))
#define USB_enable_vbus_hw_control()	(OTGCON &= ~(1 << VBUSHWC))
#define USB_disable_vbus_hw_control()	(OTGCON |= (1 << VBUSHWC))
#define is_USB_vbus_enabled()			((OTGCON & (1 << VBUSREQ)) ? TRUE : FALSE)
#define is_USB_hnp()					((OTGCON & (1 << HNPREQ)) ? TRUE : FALSE)
#define is_USB_device_srp()				((OTGCON & (1 << SRPREQ)) ? TRUE : FALSE)
#define USB_enable_suspend_time_out_interrupt()		(OTGIEN |= (1 << STOE))
#define USB_enable_hnp_error_interrupt()			(OTGIEN |= (1 << HNPERRE))
#define USB_enable_role_exchange_interrupt()		(OTGIEN |= (1 << ROLEEXE))
#define USB_enable_bconnection_error_interrupt()	(OTGIEN |= (1 << BCERRE))
#define USB_enable_vbus_error_interrupt()			(OTGIEN |= (1 << VBERRE))
#define USB_enable_srp_interrupt()					(OTGIEN |= (1 << SRPE))
#define USB_disable_suspend_time_out_interrupt()	(OTGIEN &= ~(1 << STOE))
#define USB_disable_hnp_error_interrupt()			(OTGIEN &= ~(1 << HNPERRE))
#define USB_disable_role_exchange_interrupt()		(OTGIEN &= ~(1 << ROLEEXE))
#define USB_disable_bconnection_error_interrupt()	(OTGIEN &= ~(1 << BCERRE))
#define USB_disable_vbus_error_interrupt()			(OTGIEN &= ~(1 << VBERRE))
#define USB_disable_srp_interrupt()					(OTGIEN &= ~(1 << SRPE))
#define USB_ack_suspend_time_out_interrupt()		(OTGIEN &= ~(1 << STOI))
#define USB_ack_hnp_error_interrupt()				(OTGIEN &= ~(1 << HNPERRI))
#define USB_ack_role_exchange_interrupt()			(OTGIEN &= ~(1 << ROLEEXI))
#define USB_ack_bconnection_error_interrupt()		(OTGIEN &= ~(1 << BCERRI))
#define USB_ack_vbus_error_interrupt()				(OTGIEN &= ~(1 << VBERRI))
#define USB_ack_srp_interrupt()						(OTGIEN &= ~(1 << SRPI))
#define is_USB_suspend_time_out_interrupt()			((OTGIEN & (1 << STOI)) ? TRUE : FALSE)
#define is_USB_hnp_error_interrupt()				((OTGIEN & (1 << HNPERRI)) ? TRUE : FALSE)
#define is_USB_role_exchange_interrupt()			((OTGIEN & (1 << ROLEEXI)) ? TRUE : FALSE)
#define is_USB_bconnection_error_interrupt()		((OTGIEN & (1 << BCERRI)) ? TRUE : FALSE)
#define is_USB_vbus_error_interrupt()				((OTGIEN & (1 << VBERRI)) ? TRUE : FALSE)
#define is_USB_srp_interrupt()						((OTGIEN & (1 << SRPI)) ? TRUE : FALSE)

//**********      USB Device management      **********//
#define USB_initiate_remote_wake_up()			(UDCON |=  (1 << RMWKUP))
#define USB_detach()							(UDCON |=  (1 << DETACH))
#define USB_attach()							(UDCON &= ~(1 << DETACH))
#define is_USB_pending_remote_wake_up()			((UDCON & (1 << RMWKUP)) ? TRUE : FALSE)
#define is_USB_detached()						((UDCON & (1 << DETACH)) ? TRUE : FALSE)

#define USB_ack_remote_wake_up_start()			(UDINT = ~(1 << UPRSMI))
#define USB_ack_resume()						(UDINT = ~(1 << EORSMI))
#define USB_ack_wake_up()						(UDINT = ~(1 << WAKEUPI))
#define USB_ack_reset()							(UDINT = ~(1 << EORSTI))
#define USB_ack_sof()							(UDINT = ~(1 << SOFI))
#define USB_ack_suspend()						(UDINT = ~(1 << SUSPI))
#define is_USB_remote_wake_up_start()			((UDINT & (1 << UPRSMI)) ? TRUE : FALSE)
#define is_USB_resume()							((UDINT & (1 << EORSMI)) ? TRUE : FALSE)
#define is_USB_wake_up()						((UDINT & (1 << WAKEUPI)) ? TRUE : FALSE)
#define is_USB_reset()							((UDINT & (1 << EORSTI)) ? TRUE : FALSE)
#define is_USB_sof()							((UDINT & (1 << SOFI)) ? TRUE : FALSE)
#define is_USB_suspend()						((UDINT & (1 << SUSPI)) ? TRUE : FALSE)

#define USB_enable_remote_wake_up_interrupt()	(UDIEN |= (1 << UPRSME))
#define USB_enable_resume_interrupt()			(UDIEN |= (1 << EORSME))
#define USB_enable_wake_up_interrupt()			(UDIEN |= (1 << WAKEUPE))
#define USB_enable_reset_interrupt()			(UDIEN |= (1 << EORSTE))
#define USB_enable_sof_interrupt()				(UDIEN |= (1 << SOFE))
#define USB_enable_micro_sof_interrupt()		(UDIEN |= (1 << MSOFE))
#define USB_enable_suspend_interrupt()			(UDIEN |= (1 << SUSPE))
#define USB_disable_remote_wake_up_interrupt()	(UDIEN &= ~(1 << UPRSME))
#define USB_disable_resume_interrupt()			(UDIEN &= ~(1 << EORSME))
#define USB_disable_wake_up_interrupt()			(UDIEN &= ~(1 << WAKEUPE))
#define USB_disable_reset_interrupt()			(UDIEN &= ~(1 << EORSTE))
#define USB_disable_sof_interrupt()				(UDIEN &= ~(1 << SOFE))
#define USB_disable_micro_sof_interrupt()		(UDIEN &= ~(1 << MSOFE))
#define USB_disable_suspend_interrupt()			(UDIEN &= ~(1 << SUSPE))

#define USB_enable_address()					(UDADDR |= (1 << ADDEN))
#define USB_disable_address()					(UDADDR &= ~(1 << ADDEN))
#define USB_configure_address(addr)				(UDADDR = (UDADDR & (1 << ADDEN)) | ((uint8_t)addr & EP_MASK_UADD))

#define USB_frame_number()						((uint16_t)((((uint16_t)UDFNUMH) << 8) | ((uint16_t)UDFNUML)))
#define USB_micro_frame_number()				((uint8_t)(UDMFN & (1 << MFNUM))
#define is_USB_frame_number_crc_error()			((UDMFN & (1 << FNCERR)) ? TRUE : FALSE)

//**********      General endpoint management      **********//
#define USB_select_endpoint(ep)					(UENUM = (uint8_t)ep )
#define USB_reset_endpoint(ep)					(UERST = 1 << (uint8_t)ep, UERST = 0)
#define USB_enable_endpoint()					(UECONX |= (1 << EPEN))
#define USB_enable_stall_handshake()			(UECONX |= (1 << STALLRQ))
#define USB_reset_data_toggle()					(UECONX |= (1 << RSTDT))
#define USB_disable_endpoint()					(UECONX &= ~(1 << EPEN))
#define USB_disable_stall_handshake()			(UECONX |= (1 << STALLRQC))
#define USB_select_epnum_for_cpu()				(UECONX &= ~(1 << EPNUMS))
#define is_USB_endpoint_enabled()				((UECONX & (1 << EPEN)) ? TRUE : FALSE)
#define is_USB_endpoint_stall_requested()		((UECONX & (1 << STALLRQ)) ? TRUE : FALSE)

#define USB_configure_endpoint_type(type)		(UECFG0X = (UECFG0X & ~(1 << EP_MASK_EPTYPE)) | ((uint8_t)type << 6))
#define USB_enable_iso_autoswitch()				(UECFG0X |= (1 << ISOSW))
#define USB_enable_autoswitch()					(UECFG0X |= (1 << AUTOSW))
#define USB_enable_nyet()						(UECFG0X &= ~(1 << NYETDIS))
#define USB_disable_iso_autoswitch()			(UECFG0X &= ~(1 << ISOSW))
#define USB_disable_autoswitch()				(UECFG0X &= ~(1 << AUTOSW))
#define USB_disable_nyet()						(UECFG0X |= (1 << NYETDIS))
#define USB_configure_endpoint_direction(dir)	(UECFG0X = (UECFG0X & ~(1 << EPDIR))  | ((uint8_t)dir))

#define USB_configure_endpoint_size(size)		(UECFG1X = (UECFG1X & ~EP_MASK_EPSIZE) | ((uint8_t)size << 4))
#define USB_configure_endpoint_bank(bank)		(UECFG1X = (UECFG1X & ~EP_MASK_EPBK)   | ((uint8_t)bank << 2))
#define USB_allocate_memory()					(UECFG1X |= (1 << ALLOC))
#define USB_unallocate_memory()					(UECFG1X &= ~(1 << ALLOC))



#define USB_build_ep_config0(type, dir, nyet)  ((type<<6) | (nyet<<1) | (dir))
#define USB_build_ep_config1(size, bank     )  ((size<<4) | (bank<<2)        )
#define USB_configure_endpoint(num, type, dir, size, bank, nyet)             \
                                    ( USB_select_endpoint(num),              \
                                      usbConfigEp(USB_build_ep_config0(type, dir, nyet),\
                                                    USB_build_ep_config1(size, bank)    ))


#define USB_ack_overflow_interrupt()			(UESTA0X &= ~(1 << OVERFI))
#define USB_ack_underflow_interrupt()			(UESTA0X &= ~(1 << UNDERFI))
#define USB_ack_zlp()							(UESTA0X &= ~(1 << ZLPSEEN))
#define USB_data_toggle()						((UESTA0X&EP_MASK_DTSEQ) >> 2)
#define USB_nb_busy_bank()						(UESTA0X & EP_MASK_NBUSYBK)
#define is_USB_one_bank_busy()					((UESTA0X & EP_MASK_NBUSYBK) == 0 ? FALSE : TRUE)
#define is_endpoint_configured()				((UESTA0X & (1 << CFGOK))   ? TRUE : FALSE)
#define is_USB_overflow()						((UESTA0X & (1 << OVERFI))  ? TRUE : FALSE)
#define is_USB_underflow()						((UESTA0X & (1 << UNDERFI)) ? TRUE : FALSE)
#define is_USB_zlp()							((UESTA0X & (1 << ZLPSEEN)) ? TRUE : FALSE)

#define USB_control_direction()					((UESTA1X & (1 << CTRLDIR)) >> 2)
#define USB_current_bank()						( UESTA1X & EP_MASK_CURRBK)

#define USB_ack_fifocon()						(UEINTX &= ~(1 << FIFOCON))
#define USB_ack_nak_in()						(UEINTX &= ~(1 << NAKINI))
#define USB_ack_nak_out()						(UEINTX &= ~(1 << NAKOUTI))
#define USB_ack_receive_setup()					(UEINTX &= ~(1 << RXSTPI))
#define USB_ack_receive_out()					(UEINTX &= ~(1 << RXOUTI), USB_ack_fifocon())
#define USB_ack_stalled()						(EP_MASK_STALLEDI = 0)
#define USB_ack_in_ready()						(UEINTX &= ~(1 << TXINI), USB_ack_fifocon())
#define USB_kill_last_in_bank()					(UENTTX |= (1 << RXOUTI)) 
#define is_USB_read_enabled()					(UEINTX&(1 << RWAL))
#define is_USB_write_enabled()					(UEINTX&(1 << RWAL))
#define is_USB_read_control_enabled()			(UEINTX&(1 << TXINI))
#define is_USB_receive_setup()					(UEINTX&(1 << RXSTPI))
#define is_USB_receive_out()					(UEINTX&(1 << RXOUTI))
#define is_USB_in_ready()						(UEINTX&(1 << TXINI))
#define is_USB_in_nak_irq()						(UEINTX&(1 << NAKINI))
#define USB_send_in()							(UEINTX &= ~(1 << FIFOCON))
#define USB_send_control_in()					(UEINTX &= ~(1 << TXINI))
#define USB_free_out_bank()						(UEINTX &= ~(1 << FIFOCON))
#define USB_ack_control_out()					(UEINTX &= ~(1 << RXOUTI))

#define USB_enable_flow_error_interrupt()		(UEIENX |= (1 << FLERRE))
#define USB_enable_nak_in_interrupt()			(UEIENX |= (1 << NAKINE))
#define USB_enable_nak_out_interrupt()			(UEIENX |= (1 << NAKOUTE))
#define USB_enable_receive_setup_interrupt()	(UEIENX |= (1 << RXSTPE))
#define USB_enable_receive_out_interrupt()		(UEIENX |= (1 << RXOUTE))
#define USB_enable_stalled_interrupt()			(UEIENX |= (1 << STALLEDE))
#define USB_enable_in_ready_interrupt()			(UEIENX |= (1 << TXINE))
#define USB_disable_flow_error_interrupt()		(UEIENX &= ~(1 << FLERRE))
#define USB_disable_nak_in_interrupt()			(UEIENX &= ~(1 << NAKINE))
#define USB_disable_nak_out_interrupt()			(UEIENX &= ~(1 << NAKOUTE))
#define USB_disable_receive_setup_interrupt()	(UEIENX &= ~(1 << RXSTPE))
#define USB_disable_receive_out_interrupt()		(UEIENX &= ~(1 << RXOUTE))
#define USB_disable_stalled_interrupt()			(UEIENX &= ~(1 << STALLEDE))
#define USB_disable_in_ready_interrupt()		(UEIENX &= ~(1 << TXINE))

#define USB_read_byte()							(UEDATX)
#define USB_write_byte(byte)					(UEDATX = (uint8_t)byte)

#define USB_byte_counter()						((((uint16_t)UEBCHX) << 8) | (UEBCLX))
#define USB_byte_counter_8()					((uint8_t)UEBCLX)

#define USB_interrupt_flags()					(UEINT != 0x00)
#define USB_clear_ep_interrupt_flag(ep)			(UEINT &= ~(1 << (uint8_t) ep))
#define is_USB_endpoint_event()					(USB_interrupt_flags())

// ADVANCED MACROS
#define USB_select_ep_for_cpu(ep)				(USB_select_epnum_for_cpu(), USB_select_endpoint(ep))

//! USB Host management
//! These macros manage the USB Host controller.
#define host_enable()                          (USBCON |= (1<<HOST))
#define host_enable_sof()                      (UHCON |= (1<<SOFEN))
#define host_disable_sof()                     (UHCON &= ~(1<<SOFEN))
#define host_send_reset()                      (UHCON |= (1<<RESET))
#define host_is_reset()                        ((UHCON & (1<<RESET)) ? TRUE : FALSE)
#define host_send_resume()                     (UHCON |= (1<<RESUME))
#define host_is_resume()                       ((UHCON & (1<<RESUME)) ? TRUE : FALSE)
#define host_is_sof()                          ((UHINT & (1<<HSOFI)) ? TRUE : FALSE)
#define host_ack_sof()                         (UHINT &= ~(1<<HSOFI))
#define host_vbus_request()                    (OTGCON |= (1<<VBUSREQ))
#define host_configure_address(addr)           (UHADDR = addr & MSK_HADDR)

//! General pipe management
//! These macros manage the common features of the pipes
#define host_select_pipe(p)                    (UPNUM = (uint8_t)p)
#define host_enable_pipe()                     (UPCONX |= (1<<PEN))
#define host_disable_pipe()                    (UPCONX &= ~(1<<PEN))
#define host_set_token_setup()                 (UPCFG0X =  UPCFG0X & ~(1<<TOKEN_SETUP))
#define host_set_token_in()                    (UPCFG0X = (UPCFG0X & ~(1<<TOKEN_SETUP)) | (1<<TOKEN_IN)
#define host_set_token_out()                   (UPCFG0X = (UPCFG0X & ~(1<<TOKEN_SETUP)) | (1<<TOKEN_OUT)
#define host_set_interrupt_frequency(frq)      (UPCFG2X = (uint8_t)frq)
#define is_pipe_configured()                   ((UPSTAX &  (1<<CFGOK))   ? TRUE : FALSE)
#define host_write_byte(dat)                   (UPDATX = dat)
#define host_read_byte(dat)                    (UPDATX)
#define host_unfreeze_pipe()                   (UPCONX |=  (1<<PFREEZE))
#define host_freeze_pipe()                     (UPCONX &= ~(1<<PFREEZE))
#define host_is_setup_sent()                   (TXSTPI)
#define host_is_control_in_received()          (FIFOCON)
#define host_is_control_out_sent()             (FIFOCON)
#define host_is_stall()                        (RXSTALLI)
#define host_is_error()                        (PERRI)
#define host_send_setup()                      (FIFOCON = 0)
#define host_send_control_in()                 (FIFOCON = 0)
#define host_send_control_out()                (FIFOCON = 0)
#define host_ack_control_out()                 (TXOUTI = 0)
#define host_ack_control_in()                  (RXINI = 0)
#define host_standard_in_mode()                (UPCONX &= ~(1<<INMODE))
#define host_continuous_in_mode()              (UPCONX |=  (1<<INMODE))
#define host_in_request_number(in_num)         (UPINRQX = (uint8_t)in_num)
#define host_data_length_uint8_t()             (UPBCLX)
#define host_data_length_uint16_t()            ((((uint16_t)UPBCHX)<<8) | UPBCLX)
#define host_get_pipe_length()                 ((uint16_t)0x08 << ((UPCFG1X & (1<<PSIZE))>>4))
#define host_error_status()                    (UPERRX & (1<<ERROR))
#define host_ack_all_errors()                  (UPERRX = 0x00)

//**********      Declarations      **********//
#define EVT_USB                        0x60               // USB Event
#define EVT_USB_POWERED               (EVT_USB+1)         // USB plugged
#define EVT_USB_UNPOWERED             (EVT_USB+2)         // USB un-plugged
#define EVT_USB_DEVICE_FUNCTION       (EVT_USB+3)         // USB in device
#define EVT_USB_HOST_FUNCTION         (EVT_USB+4)         // USB in host
#define EVT_USB_SUSPEND               (EVT_USB+5)         // USB suspend
#define EVT_USB_WAKE_UP               (EVT_USB+6)         // USB wake up
#define EVT_USB_RESUME                (EVT_USB+7)         // USB resume
#define EVT_USB_RESET                 (EVT_USB+8)         // USB hight speed

//**********      Functions      **********//
void usbCommonInit(void);

#endif /*USB_COMMON_H_*/

