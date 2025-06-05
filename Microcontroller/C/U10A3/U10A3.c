/*
 * April 2021, OST
 * initializes USB
 * sends state of dip switch to host in the form of "switch[1|2|3|4] [pressed|released]
 * receives commands from the host in the form of "led[1|2|3|4] [on|off] and actions on the led's accordingly
 * function "countChar" returns number of occurrences of a given character in a string
 */ 

#include <stdio.h>          // für sprintf
#include <string.h>         // für strstr
#include <avr/io.h>         // für Registerzugriffe
#include <avr/interrupt.h>  // für sei()
#include "usb.h"            // eigene USB-Schnittstelle

// Zählt, wie oft Zeichen ch im String str vorkommt
uint8_t countChar(uint8_t str[], uint8_t ch) {
	uint8_t i = 0, count = 0;
	while (str[i] != 0) {            // bis Nullzeichen
		if (str[i] == ch) count++;   // Treffer zählen
		i++;
	}
	return count;
}

int main(void) {
	uint8_t prevIn = PIND, k = 0;        // prevIn speichert vorherigen Schalterzustand, k = Eingabeindex
	char str[32], outStr[32];            // Eingabepuffer, Ausgabe-String
	DDRB |= 0xf;                         // PB0–PB3 als Ausgang (LEDs)
	usbInit();                           // USB-System starten
	sei();                               // Globale Interrupts aktivieren
	
	while(1) {
		uint8_t ch;
		
		// Zeichen vom Host empfangen
		if (usbGetAvailableReceive() > 0) {
			ch = usbReadChar();         // nächstes Zeichen lesen
			if (ch == '\r') {           // Befehl abgeschlossen?
				str[k] = 0;             // String terminieren
				if (strstr(str, "led")) {         // beginnt mit "led"?
					uint8_t pin = str[3] - '1';    // z.?B. "led2" ? '2' - '1' = 1 (PB1)
					if (strstr(str, "on"))         // enthält "on"?
						PORTB |= 1 << pin;         // LED einschalten
					if (strstr(str, "off"))        // enthält "off"?
						PORTB &= ~(1 << pin);      // LED ausschalten
				}
				k = 0;                 // Eingabepuffer zurücksetzen
			} else 
				str[k++] = ch;        // Zeichen in Eingabepuffer ablegen
		}

		// Schalterstatus prüfen (obere 4 Bit von Port D)
		uint8_t in = (PIND & 0xf0) ^ prevIn;   // Änderung feststellen
		if (in) {
			uint8_t pin = 0; 
			while (!(in & (1 << pin))) pin++;   // betroffener Pin finden
			if (PIND & (1 << pin))
				sprintf(outStr, "switch %d released\r\n", pin-3); // HIGH ? released
			else
				sprintf(outStr, "switch %d pressed\r\n", pin-3);  // LOW ? pressed
			usbWriteString((uint8_t*)outStr);      // Meldung senden
			prevIn = PIND & 0xf0;                  // neuen Zustand merken
		}
	}
}
