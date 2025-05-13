/*
 * Feb 2021, OST 
 * Fills memory locations from address 800 to 850.
 */ 

.INCLUDE "m32U4def.inc"

.EQU		adr=800		// define start address

.ORG		0			// set start address of code
start:					// initialize
	ldi		XL,LOW(adr)
	ldi		XH,HIGH(adr)
	ldi		r16,10
	ldi		r17,3
	ldi		r18,50

loop:	
	st		X+,r16		// store indirect with postincrement
	add		r16,r17
	dec		r18
	brne	loop

end:					// endless loop
	rjmp	end