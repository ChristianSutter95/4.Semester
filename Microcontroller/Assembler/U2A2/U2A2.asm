/*
 * Feb. 2025, OST 
 * Displays a bit pattern on pins PB0..PB3
 */ 

.INCLUDE "m32U4def.inc"

.ORG		0			// set start address of code
start:					// initialize
	ldi		r16,0x0f
	out		DDRB,r16
	ldi		r16,0x05
	out		PORTB,r16

loop:					// endless loop
	rjmp	loop
