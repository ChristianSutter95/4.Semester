/*
 * Feb. 2025, OST
 * Increment a register up to a certain limit, then stops
 */

.INCLUDE "m32U4def.inc"

.ORG		0			// set start address of code
start:					// initialize
	ldi		r16,4
	clr		r17

loop:					
	add		r17,r16
	cpi		r17,32
	brne	loop

end:					// endless loop
	rjmp	end
