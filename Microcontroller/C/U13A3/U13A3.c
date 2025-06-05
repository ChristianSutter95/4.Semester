/*
 * Aufgabe 3 (Erweiterung von Aufgabe 2)
 * - Temperaturmessung mit 0.1 °C Genauigkeit (Fixpunktarithmetik)
 * - INT0-Interrupt löst Kalibrationsereignis aus (z.?B. bei 25 °C)
 * - Ereignis wird über LED (PB0) angezeigt
 */

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "usb.h"

const uint16_t table[13] = {
	75, 100, 131, 169, 214, 265, 322,
	384, 447, 512, 574, 633, 687
};

volatile uint8_t go;
const uint8_t scale = 5;  // Skalierung für 0.1 °C ? 2? = 32

// ADC Interrupt: nach Messung
ISR(ADC_vect) {
	go = 1;
}

// Timer Interrupt: alle 1 s
ISR(TIMER3_COMPA_vect) {
	go = 1;
}

// === INT0 Interrupt: Trigger durch externes Signal (z.?B. Büroklammer an +5V)
ISR(INT0_vect) {
	PORTB |= 0x01;  // LED an PB0 einschalten als Ereignissignal
}

int main() {
	uint8_t str[32];
	uint16_t val, diff, temp;
	int8_t tx, i;

	go = 0;
	usbInit();     // USB initialisieren
	DDRB = 0x01;   // PB0 als Ausgang für LED

	// === Timer 3 Konfiguration ===
	TCCR3A = 0;
	TCCR3B = 0x0C;     // CTC-Modus, Prescaler = 256
	OCR3A = 31250;     // 1s bei 8 MHz
	TIMSK3 |= 0x02;    // Timer Interrupt aktivieren

	// === ADC Konfiguration ===
	ADCSRA = 0x0F;     // ADC aktiv, Interrupt an, Prescaler = 128
	ADMUX  = 0x40;     // Kanal ADC0, Referenz = AVcc

	// === INT0 Konfiguration (extern, fallende Flanke) ===
	EICRA = (1 << ISC01);  // INT0 aktiviert bei fallender Flanke
	EIMSK = (1 << INT0);   // INT0 aktivieren

	sei();  // Globale Interruptfreigabe

	while (1) {
		// === 1. Sleep bis Timer-Interrupt (1 s) ===
		set_sleep_mode(SLEEP_MODE_IDLE);
		ADCSRA &= ~0x80;  // ADC deaktivieren
		while (!go) sleep_mode();
		go = 0;

		// === 2. ADC aktivieren & wandeln im ADC-Sleep-Modus ===
		ADCSRA |= 0x80;  // ADC aktivieren
		set_sleep_mode(SLEEP_MODE_ADC);
		while (!go) sleep_mode();
		go = 0;

		val = ADCW;

		// === 3. Temperatur interpolieren (0.1 °C) ===
		i = 0;
		while (val > table[i]) i++;
		tx = -25 + 5 * i;
		diff = (5 << scale) * (val - table[i - 1]) / (table[i] - table[i - 1]);
		temp = (tx << scale) + diff;

		// === 4. Ausgabe über USB ===
		sprintf((char*)str, "T = %3d.%1d%cC\r\n",
			temp >> scale,
			((temp & 0x1F) * 10) >> scale,
			176);
		usbWriteString(str);
	}

	return 0;
}
