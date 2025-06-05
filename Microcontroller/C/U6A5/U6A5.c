/*
 * March 2021, OST 
 * Reads state of button on PD6. When state has changed 3 times to positive level the output on PB1 toggles
 */ 

#include <avr/io.h> // Zugriff auf I/O-Register

int main() {
	char count = 0;            // Z�hlt positive Flanken
	char in, prevIn;           // aktueller und vorheriger Portzustand

	DDRB |= 0x02;              // PB1 (LED) als Ausgang
	DDRD &= ~0x40;             // PD6 (Taster) als Eingang
	prevIn = PIND;             // Anfangszustand merken

	while (1) {
		in = PIND;                                     // Port D einlesen
		if ((in ^ prevIn) & 0x40) {                    // Pr�fen: hat sich PD6 ge�ndert?
			prevIn = in;                               // neuen Zustand merken
			if (in & 0x40) {                           // nur wenn neuer Zustand HIGH ist ? positive Flanke
				count++;                               // Z�hler inkrementieren
				if (count == 3) {                      // bei dritter positiver Flanke
					PORTB = PINB ^ 0x02;               // LED toggeln
					count = 0;                         // Z�hler zur�cksetzen
				}
			}
		}
	}

	return 0;
}
