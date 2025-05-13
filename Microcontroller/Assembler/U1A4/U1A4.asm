/*
 * Jan 2021, OST 
 * Toggles bits in register
 */ 

.INCLUDE "m32U4def.inc"

.ORG		0			// set start address of code
start:					// initialize
	ldi		r16,0xff
	clc

loop:					// endless loop
	ror		r16
	sbrc	r16,0
	brne	loop
	rjmp	start
