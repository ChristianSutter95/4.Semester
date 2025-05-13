/*
 * Feb 2021, OST 
 * Subroutine which toggles memory location.
 */ 

.INCLUDE "m32U4def.inc"

.EQU	val = 0x100
.EQU	mask = 0xf0

.ORG		0				// set start address of code
start:						// initialize	
	// init stack pointer
	ldi		r16,HIGH(RAMEND)
	out		SPH,r16
	ldi		r16,LOW(RAMEND)
	out		SPL,r16
	ldi		r16,0xaa
	sts		val,r16
	ldi		r20,mask

loop:
	rcall toggle
	rjmp	loop

toggle:	
	push r16
	lds r16,val
	eor r16,r20
	sts val,r16
	pop r16
	ret