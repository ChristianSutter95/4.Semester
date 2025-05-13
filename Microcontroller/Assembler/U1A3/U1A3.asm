/*
 * Jan 2021, OST 
 * Clears bits in register
 */ 

.INCLUDE "m32U4def.inc"

.ORG		0			// set start address of code
start:					// initialize
	ldi		r16,0xff

loop:					// endless loop
	lsr		r16
	sbrc	r16,0
	brne	loop
	rjmp	start
