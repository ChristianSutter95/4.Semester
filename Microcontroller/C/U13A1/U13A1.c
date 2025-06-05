/*
 * Aufgabe 1: Temperaturmessung mit 1 °C Genauigkeit
 * - Misst ADC-Wert (NTC)
 * - Verwendet Lookup-Tabelle (ADC-Werte für bestimmte Temperaturen)
 * - Führt lineare Interpolation zwischen zwei Werten aus
 * - Gibt Temperatur (auf 1 °C genau) via USB aus
 */

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "usb.h"

// Lookup-Tabelle: ADC-Werte bei -20°C bis +40°C in 5°C-Schritten
const uint16_t table[13] = {
	75, 100, 131, 169, 214, 265, 322,
	384, 447, 512, 574, 633, 687
};

volatile uint8_t go;

ISR(ADC_vect) {
	go = 1;  // Flag für Abschluss der ADC-Wandlung
}

ISR(TIMER3_COMPA_vect) {
	go = 1;  // Flag für 1-Sekunden-Takt
}

int main() {
	uint8_t str[32];
	uint16_t val, diff, temp;
	int8_t i, tx;

	go = 0;
	usbInit();     // USB initialisieren
	DDRB = 1;      // PB0 als Ausgang (z.?B. für Debug-LED, optional)

	// === Timer 3 Konfiguration ===
	TCCR3A = 0;
	TCCR3B = 0x0C;     // CTC-Modus, Prescaler = 256
	OCR3A = 31250;     // 1s bei 8 MHz
	TIMSK3 |= 0x02;    // Timer Compare Match A Interrupt aktivieren

	// === ADC Konfiguration ===
	ADCSRA = 0x0F;     // ADC aktivieren, Interrupt erlauben, Prescaler = 128
	ADMUX = 0x40;      // ADC0, Referenz = AVcc

	sei();  // Globale Interrupts freigeben

	while (1) {
		// Schritt 1: Warten bis 1 Sekunde vergangen
		set_sleep_mode(SLEEP_MODE_IDLE);
		ADCSRA &= ~0x80;  // ADC deaktivieren
		while (!go) sleep_mode();
		go = 0;

		// Schritt 2: ADC aktivieren, messen und aufwecken lassen
		ADCSRA |= 0x80;  // ADC aktivieren
		set_sleep_mode(SLEEP_MODE_ADC);
		while (!go) sleep_mode();
		go = 0;

		val = ADCW;  // ADC-Wert übernehmen

		// Schritt 3: Lookup & Interpolation
		i = 0;
		while (val > table[i]) i++;
		tx = -25 + 5 * i;  // Temperatur an oberem Stützpunkt

		// Lineare Interpolation:
		diff = 5 * (val - table[i - 1]) / (table[i] - table[i - 1]);  // Delta-T in °C
		temp = tx + diff;  // Temperatur in °C

		// Schritt 4: Ausgabe via USB
		sprintf((char*)str, "T = %2d%cC\r\n", temp, 176);  // Ausgabe mit °C-Zeichen
		usbWriteString(str);
	}

	return 0;
}
