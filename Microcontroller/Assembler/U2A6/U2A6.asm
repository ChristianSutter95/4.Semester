/*
 * Feb. 2025, OST 
 * Output on PB1 toggles after three positive edges on PD6.
 */ 

.INCLUDE "m32U4def.inc"

.DEF		count=r20	// defines registers for variables
.DEF		oldIn=r21
.DEF		input=r22

.ORG		0			// set start address of code
start:					// initialize
	ldi		count,5
	in		oldIn,PIND
	sbi		DDRB,1
	cbi		DDRD,6

loop:	
	in		input,PIND
	eor		input,oldIn
	sbrs	input,6		// check if bit 6 changed
	rjmp	loop
	in		oldIn,PIND	// update oldIn
	sbrs	input,6		// test for positive edge
	rjmp	loop
	dec		count
	brne	loop
	ldi		count,5
	in		r16,PINB	// toggle output
	ldi		r17,0x02
	eor		r16,r17
	out		PORTB,r16
	rjmp	loop