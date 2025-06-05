#ifndef PLL_H_
#define PLL_H_

/*
 * Definitions for the PLL in the system clock unit.
 * The PLL is used to generate the high frequency USB clock.
 * 22.3.2017, Urs Graf
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#if defined(__AVR_ATmega16U4__) || defined(__AVR_ATmega32U4__)
	#if (F_CPU==8000000)
		#define PLL_start_auto()	(PLLFRQ = ((1<<PLLUSB) | (1<<PLLTM1) | (1<<PDIV3) | (1<<PDIV1)), PLLCSR = (PLLCSR | (1<<PLLE)))
	#else
		#error (F_CPU must be defined in the makefile)
	#endif
#elif defined(__AVR_AT90USB1287__)
	#if (F_CPU==8000000)
		#define PLL_start_auto()   (PLLCSR = ((1<<PLLP1) | (1<<PLLP0) | (1<<PLLE)))
	#else
		#error (F_CPU must be defined in the makefile)
	#endif
#else
	#error (no supported PLL settings for this processor type)
#endif


// Test PLL lock, return 1 when PLL locked
#define is_PLL_ready()		(PLLCSR & (1<<PLOCK) )

// Test PLL lock bit and wait until lock is set
#define wait_PLL_ready()	while (!(PLLCSR & (1<<PLOCK)))

// Stop the PLL
#define stop_pll()			(PLLCSR  &= (~(1<<PLLE)) )       

#endif /*PLL_H_*/
