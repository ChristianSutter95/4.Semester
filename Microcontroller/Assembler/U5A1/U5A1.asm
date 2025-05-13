/*
 * March 2021, OST 
 * Blinks LED on PB0 with frequency of 2Hz
 * Uses timer0 interrupt
 * Timer0 runs with 8Mhz / 1024 -> 128us per tick
 * counts from 178 to 256 -> 78 * 128us = 9.98ms
 */ 
 
 .INCLUDE "m32U4def.inc"

.EQU	t0Start = 178		// 256-78
.EQU	cntStart = 25
.EQU	cntAddr = 0x100		// counter variable at start of ram


.ORG 0						// reset vector
	rjmp	start

.ORG OVF0addr				// timer 0 overflow vector
	rjmp	T0ISR

.ORG 0x50					// start of main program
start:						// initialize	
	ldi		r16,HIGH(RAMEND)// init stack pointer
	out		SPH,r16
	ldi		r16,LOW(RAMEND)
	out		SPL,r16
	ldi		r16,0x01		// make PB0 output
	out		DDRB,r16
	ldi		r16,5			// prescale by 1024
	out		TCCR0B,r16		
	ldi		r16,t0Start		// load timer0
	out		TCNT0,r16	
	ldi		r16,1
	sts		TIMSK0,r16		// enable timer ints
	ldi		r16,cntStart	// load counter
	sts		cntAddr,r16
	sei						// enable ints globally
loop:
	rjmp	loop

// timer 0 overflow interrupt service routine
T0ISR:						// once per 10ms
	push	r16				// context save
	push	r17
	in		r16,SREG
	push	r16
	ldi		r16,t0Start		// reload timer0
	out		TCNT0,r16
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