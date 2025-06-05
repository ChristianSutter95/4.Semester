/*
 * Aufgabe 2: Temperaturmessung mit 0.1 °C Genauigkeit
 * - Verwendung von Fixpunktarithmetik (Skalierungsfaktor = 32)
 * - Ausgabe über USB mit einer Nachkommastelle (0.1 °C)
 */

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "usb.h"

// Lookup-Tabelle mit ADC-Werten (für -20°C bis +40°C in 5°C-Schritten)
const uint16_t table[13] = {
	75, 100, 131, 169, 214, 265, 322,
	384, 447, 512, 574, 633, 687
};

volatile uint8_t go;
const uint8_t scale = 5;  // Skalierungsfaktor für 0.1 °C ? 2? = 32

ISR(ADC_vect) {
	go = 1;  // ADC-Messung abgeschlossen
}

ISR(TIMER3_COMPA_vect) {
	go = 1;  // Timerereignis (1s)
}

int main() {
	uint8_t str[32];
	uint16_t val, diff, temp;
	int8_t tx, i;

	go = 0;
	usbInit();    // USB initialisieren
	DDRB = 1;     // PB0 als Ausgang (LED optional)

	// === Timer 3 Konfiguration ===
	TCCR3A = 0;
	TCCR3B = 0x0C;     // CTC-Modus, Prescaler = 256
	OCR3A = 31250;     // 1s bei 8 MHz
	TIMSK3 |= 0x02;    // Timer Interrupt aktivieren

	// === ADC Konfiguration ===
	ADCSRA = 0x0F;     // ADC aktiv, Interrupt an, Prescaler = 128
	ADMUX  = 0x40;     // Kanal ADC0, Referenz = AVcc

	sei();  // Interrupts global freigeben

	while (1) {
		// === 1. Sleep bis Timerereignis ===
		set_sleep_mode(SLEEP_MODE_IDLE);
		ADCSRA &= ~0x80;  // ADC deaktivieren
		while (!go) sleep_mode();
		go = 0;

		// === 2. ADC aktivieren & wandeln im Sleep-Modus ===
		ADCSRA |= 0x80;  // ADC aktivieren
		set_sleep_mode(SLEEP_MODE_ADC);
		while (!go) sleep_mode();
		go = 0;

		val = ADCW;  // Messwert übernehmen

		// === 3. Interpolation (Fixpunktarithmetik) ===
		i = 0;
		while (val > table[i]) i++;        // Intervall finden
		tx = -25 + 5 * i;                  // Temperatur am oberen Stützpunkt
		diff = (5 << scale) * (val - table[i - 1]) / (table[i] - table[i - 1]);
		// Skaliertes ?T in Zehntelgrad berechnen

		temp = (tx << scale) + diff;      // Ergebnis (skaliert) in temp

		// === 4. Ausgabe: Ganzzahlteil und Nachkommastelle extrahieren ===
		sprintf((char*)str, "T = %3d.%1d%cC\r\n",
			temp >> scale,                        // Ganzzahlteil
			((temp & 0x1F) * 10) >> scale,         // 1 Dezimalstelle
			176);                                 // Gradzeichen
		usbWriteString(str);
	}

	return 0;
}
