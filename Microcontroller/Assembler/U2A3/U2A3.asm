/*
 * Feb. 2025, OST 
 * Reads states of buttons on PD4..PD7 and outputs this state on PB0..PB3
 */ 

.INCLUDE "m32U4def.inc"

.ORG		0			// set start address of code
start:					// initialize
 	ldi		r16,0x0f
	out		DDRB,r16
	clr		r16
	out		DDRD,r16

loop:	
	in		r16,PIND
	swap	r16
	out		PORTB,r16
	rjmp	loop
