/*
 * Feb 2021, OST 
 * Blinks a LED on PB0 with 1Hz. Processor clock is at 8MHz.
 */ 

.INCLUDE "m32U4def.inc"

.EQU		maxCount=50000	// define upper limit for counter
.EQU		loopCount=20	// define upper limit for outer loop

.ORG		0				// set start address of code
start:						// initialize
	ldi		r16,1
	out		DDRB,r16
	ldi		r18,loopCount

loop:
	ldi		XH,HIGH(maxCount)
	ldi		XL,LOW(maxCount)

count:
	sbiw	XH:XL,1			// 2 cycles
	brne	count			// 2 cycles, total 4 * 50000 = 200000 cycles
	dec		r18	
	brne	loop			// check outer loop
	ldi		r18,loopCount
	in		r17,PINB		// at 8MHz -> toggle at 4000000 cylces -> 500ms
	eor		r17,r16
	out		PORTB,r17
	rjmp	start

