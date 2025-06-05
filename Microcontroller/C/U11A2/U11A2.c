/*
 * Aufgabe 2: Kombination Timer + ADC
 * - ADC misst Spannung an ADC0 (NTC)
 * - jede Sekunde wird eine Wandlung gestartet
 * - Ergebnis wird über USB ausgegeben
 *
 * Mikrocontroller: ATmega32U4
 */

#include <avr/io.h>         // Registerdefinitionen
#include <stdio.h>          // Für sprintf()
#include <avr/interrupt.h>  // Für ISR-Definitionen
#include "usb.h"            // USB-Kommunikation

uint8_t go;  // Flag, gesetzt durch Timer-Interrupt

// ISR: Timer 3 Compare Match A
ISR(TIMER3_COMPA_vect) {
	go = 1;  // Signalisiert, dass eine Sekunde vergangen ist
}

int main() {
	uint8_t str[32];  // Puffer für String-Ausgabe

	usbInit();  // USB initialisieren

	// === Timer 3 Konfiguration ===
	TCCR3A = 0;        // Normaler Modus
	TCCR3B = 0x0C;     // CTC-Modus, Prescaler = 256
	OCR3A = 31250;     // 1s bei 8 MHz
	TIMSK3 |= 0x02;    // Compare Match A Interrupt aktivieren

	// === ADC-Konfiguration ===
	ADCSRA = 0x87;     // ADC aktivieren, Prescaler = 128 (langsamer Takt für präzise Wandlung)
	ADMUX  = 0x40;     // Kanal ADC0 (PF0), Referenzspannung = AVcc = 5V

	sei();  // Interrupts global aktivieren

	while (1) {
		while (!go);   // Warten auf Timer-Interrupt
		go = 0;        // Flag zurücksetzen

		ADCSRA |= 0x40;                  // Starte Einzelwandlung
		while (ADCSRA & 0x40);           // Warten, bis Wandlung fertig

		sprintf((char*)str, "ADC0: %4d\r\n", ADCW);  // Ergebnis formatieren
		usbWriteString(str);                         // Über USB senden
	}

	return 0;
}
