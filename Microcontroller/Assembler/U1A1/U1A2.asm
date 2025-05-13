/*
 * Jan 2021, OST 
 * Decrement a register by three, stop when value is below -8
 */ 

.INCLUDE "m16U4def.inc"

.ORG		0			// set start address of code
start:					// initialize
	ldi		r16,3
	ldi		r17,5

loop:	
	sub		r17,r16
	cpi		r17,-8
	brpl	loop

end:					// endless loop
	rjmp	end
