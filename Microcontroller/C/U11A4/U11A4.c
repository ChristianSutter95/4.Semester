/*
 * April 2021, OST
 * reads ADC value once per second
 * calculates temperature within 5 degrees
 * outputs val to USB
 */

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "usb.h"

uint8_t go;
// Lookup-Tabelle: ADC-Werte bei festen Temperaturen (–20°C bis +40°C in 5°C-Schritten)
uint16_t const table[13] = {75, 100, 131, 169, 214, 265, 322, 384, 447, 512, 574, 633, 687};
	
ISR(TIMER3_COMPA_vect) {
	go = 1;  // 1s-Intervall-Flag setzen
}

int main() {
	uint8_t str[32], i;

	usbInit();  // USB initialisieren

	// === Timer 3 Konfiguration (1 Hz) ===
	TCCR3A = 0;
	TCCR3B = 0x0C;     // CTC-Modus, Prescaler = 256
	OCR3A = 31250;     // 1s bei 8 MHz
	TIMSK3 |= 0x02;    // Compare Match A Interrupt aktivieren

	// === ADC Konfiguration ===
	ADCSRA = 0x87;     // ADC aktivieren, Prescaler = 128
	ADMUX  = 0x40;     // Kanal ADC0 (PF0), Referenz = AVcc

	sei();  // Interrupts global aktivieren

	while (1) {
		while (!go);  // Auf Timer-Flag warten
		go = 0;

		ADCSRA |= 0x40;              // Starte Einzelmessung
		while (ADCSRA & 0x40);      // Warten auf Abschluss

		// === Temperaturzuordnung via Lookup ===
		i = 0;
		while ((i < 13) && (ADCW > table[i])) i++;  // Suche passenden Bereich

		// Berechnete Temperatur: -20°C + i·5
		sprintf((char*)str, "Temperatur: %2d C\r\n", -20 + 5*i);
		usbWriteString(str);
	}

	return 0;
}