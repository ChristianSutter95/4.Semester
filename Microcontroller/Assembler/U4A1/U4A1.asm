/*
 * Feb 2021, OST 
 * Subroutine with exact delay of 10us. Processor clock is at 8MHz.
 * Call subroutine 25000 times to get blinking frequency of 2Hz.
 */ 

.INCLUDE "m32U4def.inc"

.ORG		0				// set start address of code
start:						// initialize	
	// init stack pointer
	ldi		r16,HIGH(RAMEND)
	out		SPH,r16
	ldi		r16,LOW(RAMEND)
	out		SPL,r16
	// init port
	ldi		r16,0xff
	out		DDRB,r16

loop:	
	ldi		r20,250
loop1:
	ldi		r21,100
loop2:
	call 	delay10Us		// 4 cycles
	subi	r21,1
	brne	loop2
	subi	r20,1
	brne	loop1
	in		r17,PINB
	eor		r17,r16
	out		PORTB,r17
	rjmp	loop


// Subroutine Delay 10 µs
// ======================

// Processor runs at 8 MHz, which is 125 ns cycle time.
// A delay of 10 µs means therefore 8*10 = 80 cycles

// Call to get here:												=  4 cycles
delay10Us:                              
       push   r16			// 2 cycles
       ldi    r16,22		// 1 cycle								=  3 cycles
count:
       subi   r16,1			// 1 cycle
       brne   count			// 2 cycles ==> 3 * 21 + 2(last run)    = 65 cycles

       pop    r16			// 2 cycles
       nop					// 1 cycle
       nop					// 1 cycle
       ret					// 4 cycles								=  8 cycles
							//										-----------
							// Total cycles used					= 80 cycles
							//										===========







