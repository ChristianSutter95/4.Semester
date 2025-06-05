/*
 * Aufgabe 1: Timer 3 im CTC-Modus mit 1s-Intervall
 * Ausgabe eines Zeichens über USB einmal pro Sekunde
 * 
 * Mikrocontroller: ATmega32U4
 */

#include <avr/io.h>         // Grundlegende Registerdefinitionen
#include <stdio.h>          // Für sprintf()
#include <avr/interrupt.h>  // Für ISR-Definition
#include "usb.h"            // USB-Kommunikation (bibliotheksspezifisch)

uint8_t go;  // Flag, das von der ISR gesetzt wird, um 1s-Takt anzuzeigen

// Interrupt Service Routine: Timer 3 Compare Match A
ISR(TIMER3_COMPA_vect) {go = 1;} // Signalisiert, dass eine Sekunde vergangen ist

int main() {
	uint8_t str[32];	// String-Puffer für USB-Ausgabe

	usbInit();			// USB-Schnittstelle initialisieren

	// === Timer 3 Konfiguration ===
	TCCR3A = 0;          // Kein spezieller Modus im A-Register
	TCCR3B = 0x0C;       // CTC-Modus (WGM32=1), Prescaler = 256
	OCR3A = 31250;       // Bei 8 MHz Takt ergibt das genau 1 Sekunde
	TIMSK3 |= 0x02;      // Output Compare Match A Interrupt aktivieren

	sei();  // Globale Interrupts aktivieren

	while (1) {
		while (!go);     // Warten, bis ISR Flag setzt
		go = 0;          // Flag zurücksetzen

		sprintf((char*)str, "Tick\r\n");  // Zeichenkette vorbereiten
		usbWriteString(str);              // Ausgabe über USB
	}

	return 0;
}
