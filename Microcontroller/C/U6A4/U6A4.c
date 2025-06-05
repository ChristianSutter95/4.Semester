/*
 * March 2021, OST 
 * Reads state of button on PD6. When state has changed 5 times the output on PB1 toggles
 */ 

#include <avr/io.h> // Registerdefinitionen

int main() {
	char count = 0;           // Z�hler f�r Pegelwechsel
	char in, prevIn;          // aktueller und vorheriger Eingangswert

	DDRB |= 0x02;             // PB1 als Ausgang (LED)
	DDRD &= ~0x40;            // PD6 als Eingang (Taste)
	prevIn = PIND;            // Startwert einlesen

	while (1) {
		in = PIND;                                // aktuellen Zustand lesen
		if ((in ^ prevIn) & 0x40) {               // XOR pr�ft Zustands�nderung an Bit 6
			count++;                              // Z�hler inkrementieren
			prevIn = in;                          // vorherigen Zustand aktualisieren
			if (count == 5) {                     // bei 5 Wechseln
				PORTB = PINB ^ 0x02;              // PB1 toggeln (LED-Zustand �ndern)
				count = 0;                        // Z�hler zur�cksetzen
			}
		}
	}

	return 0;
}
