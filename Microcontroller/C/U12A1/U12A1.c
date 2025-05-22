/*
 * April 2021, OST 
 * shows if processor is in sleep mode
 * pay attention to shared resources
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

ISR(TIMER3_COMPA_vect) {PORTB ^= 1;}

int main() {
	DDRB |= 0x03;

	// init timer 3, interrupts every second
	TCCR3A = 0;
	TCCR3B = 0xc;	// CTC mode, prescaler = 256
	OCR3A = 31250;	// gives exactly one second
	TIMSK3 |= 0x02;	// enable output compare match ints
	
	set_sleep_mode(SLEEP_MODE_IDLE);
	sei();

	while (1) {
		sleep_mode();              // Prozessor schläft, wacht bei Interrupt auf
		uint8_t status = SREG;     // Statusregister sichern
		cli();                     // Interrupts deaktivieren (kritischer Abschnitt)
		PORTB ^= 2;                // Toggle Bit 1
		SREG = status;             // Ursprünglichen Status wiederherstellen (inkl. sei(), falls vorher aktiv)
	}

	return 0;
}
