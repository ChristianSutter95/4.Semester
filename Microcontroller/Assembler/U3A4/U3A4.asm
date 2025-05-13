/*
 * Feb 2021, OST 
 * Blinks a LED on PB0 with 20Hz. Processor clock is at 8MHz.
 */ 

.INCLUDE "m32U4def.inc"

.EQU		maxCount=49998		// define upper limit for counter

.ORG		0					// set start address of code
start:							// initialize
	ldi		r16,1
	out		DDRB,r16

loop:
	ldi		ZH,HIGH(maxCount)	// 1 cycle
	ldi		ZL,LOW(maxCount)	// 1 cycle

count:
	sbiw	ZH:ZL,1				// 2 cycles
	brne	count				// 2 cycles, total 4 * 49997 + 3(last run) = 199991
	in		r17,PINB			// 1 cycle		
	eor		r17,r16				// 1 cycle
	out		PORTB,r17			// 1 cycle
	nop							// 1 cycle
	nop							// 1 cycle
	rjmp	loop				// 2 cycles

								// total 199991 + 9 = 200000 cycles at 8MHz -> 25ms