/*
 * March 2021, OST 
 * Blinks LED on PB0 with frequency of 2Hz
 * Uses timer0 interrupt
 * Timer0 runs with 8Mhz / 1024 -> 128us per tick
 * Counts from 178 to 256 -> 78 * 128us = 9.98ms
 * 
 * S1..S3 on PD4..PD6 are read
 * They form a number which is multiplied by 8
 * This factor is added to the counter start (178)
 * The result is loaded into the compare match register
 * OCR0A = 178 + (S1..S3) * 8 + 1
 * PB1 is set every 10ms, and reset when a match occurs 
 */ 
 
 .INCLUDE "m32U4def.inc"

.EQU	t0Start = 178		// 256-78
.EQU	cntStart = 25
.EQU	cntAddr = 0x100		// counter variable at start of ram


.ORG 0						// reset vector
	rjmp	start

.ORG OC0Aaddr				// timer 0 compare match vector	
	rjmp	T0CMISR

.ORG OVF0addr				// timer 0 overflow vector
	rjmp	T0ISR

.ORG 0x50					// start of main program
start:						// initialize	
	ldi		r16,HIGH(RAMEND)// init stack pointer
	out		SPH,r16
	ldi		r16,LOW(RAMEND)
	out		SPL,r16
	ldi		r16,0x03		// make PB0, PB1 output
	out		DDRB,r16
	ldi		r16,5			// prescale by 1024
	out		TCCR0B,r16		
	ldi		r16,t0Start		// load timer0
	out		TCNT0,r16	
	ldi		r16,cntStart	// load counter
	sts		cntAddr,r16
	ldi		r16,t0Start+8	// load compare match register  
	out		OCR0A,r16		
	ldi		r16,3			// enable timer ints
	sts		TIMSK0,r16		
	sei						// enable ints globally
loop:
	in		r16,PIND		// read S1,S2,S3
	andi	r16,0x70		// and set compare match register 
	lsr		r16
	ldi		r17,t0Start+1	
	add		r17,r16
	out		OCR0A,r17
	rjmp	loop

// timer 0 overflow interrupt service routine
T0ISR:						// once per 10ms
	push	r16				// context save
	push	r17
	in		r16,SREG
	push	r16
	ldi		r16,t0Start		// reload timer0
	out		TCNT0,r16
	sbi		PORTB,1			// set PB1
	lds		r16,cntAddr	
	dec		r16
	sts		cntAddr,r16
	brne	restore			// end ISR if not yet 0
	ldi		r16,cntStart	// load counter
	sts		cntAddr,r16
	ldi		r16,0x01		// toggle PB0
	in		r17,PINB
	eor		r17,r16
	out		PORTB,r17
restore:
	pop		r16				// context restore
	out		SREG,r16
	pop		r17
	pop		r16
	reti

T0CMISR:
	cbi		PORTB,1			// reset PB1
	reti