/*
 * Feb. 2025, OST 
 * Reads state of button on PD6 and outputs state on PB1
 */ 

.INCLUDE "m32U4def.inc"

.ORG		0			// set start address of code
start:					// initialize
	sbi		DDRB,1
	cbi		DDRD,6

loop:	
	in		r16,PIND
	bst		r16,6
	clr		r16
	bld		r16,1
	out		PORTB,r16
	rjmp	loop


