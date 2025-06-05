/*
 * March 2021, OST 
 * Reads states of buttons on PD4..PD7 and outputs this state on PB0..PB3
 */ 

#include <avr/io.h> // Bindet I/O-Registerdefinitionen für AVR ein

int main(void) {
	char state;               // Variable zur Zwischenspeicherung des Tastenzustands

	DDRB |= 0x0f;             // Setze PB0 bis PB3 als Ausgang (0b00001111)
	DDRD = 0;                 // Setze Port D vollständig als Eingang

	while (1) {
		state = PIND;         // Lese den gesamten Port D ein
		PORTB = state >> 4;   // Verschiebe Bits 7..4 in 3..0, gib sie auf Port B aus
	}
}


