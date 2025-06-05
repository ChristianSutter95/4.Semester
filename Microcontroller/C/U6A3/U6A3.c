/*
 * March 2021, OST 
 * Reads state of button on PD6 and output this state on PB1.
 */ 

#include <avr/io.h> // Registerdefinitionen für AVR

int main() {

	DDRB |= 0x02;        // Setzt PB1 (Bit 1) als Ausgang
	DDRD &= ~0x40;       // Setzt PD6 (Bit 6) als Eingang (0x40 = 0b01000000)

	while (1) {
		char in = PIND;               // Lies den Port D ein
		if (in & 0x40)                // Prüfe, ob Bit 6 (PD6) HIGH ist
			PORTB = PINB | 0x02;      // Setze Bit 1 in PORTB (LED an)
		else
			PORTB = PINB & ~0x02;     // Lösche Bit 1 in PORTB (LED aus)
	}

	return 0;
}
