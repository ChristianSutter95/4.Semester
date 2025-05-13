/*
 * Feb. 2025, OST 
 * Clears bits in register
 */ 

.INCLUDE "m32U4def.inc"

.ORG		0			// set start address of code
start:					// initialize
	ldi		r16,15

loop:					// endless loop
	com		r16
	rjmp	loop
