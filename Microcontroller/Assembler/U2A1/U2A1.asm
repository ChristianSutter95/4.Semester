/*
 * Feb. 2025, OST 
 * Decrement a memory location by three, stop when value is below -8
 */ 

.INCLUDE "m32U4def.inc"


.EQU		var_a=0x100	// define address for variable 
.ORG		0			// set start address of code
start:					// initialize
	ldi		r16,3
	ldi		r17,5
	sts		var_a,r17

loop:	
	lds		r17,var_a
	sub		r17,r16
	sts		var_a,r17
	cpi		r17,-8
	brpl	loop
end:					// endless loop
	rjmp	end
