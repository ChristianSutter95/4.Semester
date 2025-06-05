/*
 * Aufgabe 3: Temperaturmessung mit optimiertem Sleep-Verhalten
 * - Mikrocontroller schl�ft im Idle-Modus bis Timerinterrupt
 * - ADC wird erst dann aktiviert
 * - Mikrocontroller wechselt in den ADC Noise Reduction Sleep Mode
 * - ADC-Vektor weckt ihn auf ? Temperaturberechnung �ber Lookup-Tabelle
 * - Ergebnis wird �ber USB ausgegeben
 */

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "usb.h"

volatile uint8_t go;  // Flag f�r Ablaufsteuerung

// Lookup-Tabelle (ADC-Werte f�r Temperaturen �20�C bis +40�C in 5�C-Schritten)
const uint16_t table[13] = {75, 100, 131, 169, 214, 265, 322,
	384, 447, 512, 574, 633, 687};

// ADC-Interrupt: wird ausgel�st, wenn Wandlung abgeschlossen
ISR(ADC_vect) {
	go = 1;
}

// Timer3-Interrupt: wird alle 1s ausgel�st
ISR(TIMER3_COMPA_vect) {
	go = 1;
}

int main() {
	uint8_t str[32], i;
	uint16_t val;

	go = 0;

	usbInit();  // USB initialisieren

	// === Timer 3 Konfiguration ===
	TCCR3A = 0;
	TCCR3B = 0x0C;     // CTC-Modus, Prescaler = 256
	OCR3A = 31250;     // 1s bei 8 MHz
	TIMSK3 |= 0x02;    // Timer Compare Match A Interrupt aktivieren

	// === ADC Konfiguration (aber noch nicht aktiviert) ===
	ADCSRA = 0x0F;     // ADC Interrupt enable, Prescaler = 128 (ADC ausgeschaltet!)
	ADMUX  = 0x40;     // Kanal ADC0, Referenz = AVcc

	sei();  // Interrupts global freigeben

	while (1) {
		// === 1. Sleep im Idle-Modus bis Timerinterrupt ===
		set_sleep_mode(SLEEP_MODE_IDLE);  // Idle-Modus (Timer aktiv)
		ADCSRA &= ~0x80;  // ADC deaktivieren
		while (!go) sleep_mode();  // Schlafen bis Timer-ISR
		go = 0;

		// === 2. ADC aktivieren und in "ADC Noise Reduction"-Modus schlafen ===
		ADCSRA |= 0x80;  // ADC aktivieren (ADEN)
		set_sleep_mode(SLEEP_MODE_ADC);  // Stromsparender ADC-Modus
		while (!go) sleep_mode();  // Schlafen bis ADC-Wandlung abgeschlossen
		go = 0;

		// === 3. Temperatur berechnen und ausgeben ===
		i = 0;
		while (ADCW > table[i]) i++;  // Passenden Temperaturbereich finden
		sprintf((char*)str, "Temperature: %4d\r\n", -25 + 5*i);  // Temperatur aus Tabelle
		usbWriteString(str);  // Ausgabe �ber USB
	}

	return 0;
}
