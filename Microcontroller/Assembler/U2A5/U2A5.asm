/*
 * Feb. 2025, OST 
 * Reads state of button on PD6. When state has changed 5 times the output on PB1 toggles
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
	dec		count
	brne	loop
	ldi		count,5		// restart the whole thing
	in		r16,PINB	// toggle output
	ldi		r17,0x02
	eor		r16,r17
	out		PORTB,r16
	rjmp	loop

