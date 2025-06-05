/*
 * Aufgabe 4: Temperaturmessung mit Kalibration
 * - INT0: Kalibriertemperatur (25 °C) manuell auslösen
 * - Kalibrationsfaktor wird im EEPROM gespeichert (Skalierung 2¹² = 4096)
 * - Faktor korrigiert alle zukünftigen Messungen
 */

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/eeprom.h>
#include "usb.h"

const uint16_t table[13] = {
	75, 100, 131, 169, 214, 265, 322,
	384, 447, 512, 574, 633, 687
};

volatile uint8_t go;
uint16_t corrFactor;        // Kalibrationsfaktor mit Skalierung 4096
const uint8_t scale = 5;    // Fixpunktskalierung für 0.1 °C (2? = 32)

// === INT0: Kalibration auslösen ===
ISR(INT0_vect) {
	PORTB |= 0x01;  // LED ein als Bestätigung

	// Korrekturfaktor berechnen: 512 soll gemessenen ADC-Wert korrigieren
	corrFactor = (((uint32_t)512 << 12) / ADCW);  // Skaliert mit 2¹² = 4096

	// Ins EEPROM schreiben
	eeprom_busy_wait();
	eeprom_update_word((uint16_t*)0, corrFactor);

	PORTB &= ~0x01;  // LED wieder aus
}

// === ISR: ADC fertig ===
ISR(ADC_vect) {
	go = 1;
}

// === ISR: Timer alle 1s ===
ISR(TIMER3_COMPA_vect) {
	go = 1;
}

int main() {
	uint8_t str[32];
	uint16_t val, diff, temp;
	int8_t tx, i;

	go = 0;
	usbInit();
	DDRB = 0x01;  // PB0 = LED-Ausgang

	// === Timer konfigurieren ===
	TCCR3A = 0;
	TCCR3B = 0x0C;       // CTC, Prescaler 256
	OCR3A = 31250;       // 1s bei 8 MHz
	TIMSK3 |= 0x02;

	// === ADC vorbereiten ===
	ADCSRA = 0x0F;       // aktiv, Interrupts, Prescaler = 128
	ADMUX = 0x40;        // AVcc, Kanal 0

	// === INT0 konfigurieren ===
	EICRA = (1 << ISC01);  // fallende Flanke
	EIMSK = (1 << INT0);   // aktivieren
	PORTD |= (1 << PD0);   // interner Pull-Up an INT0

	// === Kalibrierfaktor aus EEPROM lesen ===
	eeprom_busy_wait();
	corrFactor = eeprom_read_word((uint16_t*)0);
	if (corrFactor == 0xFFFF || corrFactor == 0) corrFactor = 4096;  // default 1.0

	sei();

	while (1) {
		// Sleep bis Timer
		set_sleep_mode(SLEEP_MODE_IDLE);
		ADCSRA &= ~0x80;
		while (!go) sleep_mode();
		go = 0;

		// ADC-Wandlung im Schlaf
		ADCSRA |= 0x80;
		set_sleep_mode(SLEEP_MODE_ADC);
		while (!go) sleep_mode();
		go = 0;

		// Wert korrigieren
		val = (uint16_t)(((uint32_t)ADCW * corrFactor) >> 12);  // korrigierter ADC-Wert

		// Interpolation
		i = 0;
		while (val > table[i]) i++;
		tx = -25 + 5 * i;
		diff = (5 << scale) * (val - table[i - 1]) / (table[i] - table[i - 1]);
		temp = (tx << scale) + diff;

		// Ausgabe in 0.1 °C
		sprintf((char*)str, "T = %3d.%1d%cC\r\n",
			temp >> scale,
			((temp & 0x1F) * 10) >> scale,
			176);
		usbWriteString(str);
	}

	return 0;
}
