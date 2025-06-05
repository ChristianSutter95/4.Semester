/*
 * Aufgabe 1 & 2: Sleep-Modus mit LED-Test & Zugriffsschutz bei Shared Resources
 * - LED1 (PB0) toggelt im Timerinterrupt ? Aktivitätsanzeige
 * - LED2 (PB1) toggelt in Hauptschleife ? zeigt ob Sleep-Modus aktiv genutzt wird
 * - Ohne sleep_mode(): LED2 flackert, Prozessor schläft nicht
 * - Schutz des PORTB-Zugriffs per Interruptmaskierung
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

// ISR: Timer 3 Compare Match A – wird 1× pro Sekunde ausgelöst
ISR(TIMER3_COMPA_vect) {
	PORTB ^= 1;  // LED1 (PB0) toggeln
}

int main() {
	DDRB |= 0x03;  // PB0 & PB1 als Ausgang setzen

	// === Timer 3 konfigurieren ===
	TCCR3A = 0;
	TCCR3B = 0x0C;     // CTC-Modus, Prescaler = 256
	OCR3A = 31250;     // 1s bei 8 MHz
	TIMSK3 |= 0x02;    // Compare Match A Interrupt aktivieren

	set_sleep_mode(SLEEP_MODE_IDLE);  // Idle-Modus konfigurieren
	sei();  // Interrupts global aktivieren

	while (1) {
		sleep_mode();  // Aktivieren für echte Sleep-Analyse

		// Zugriffsschutz: kritischer Zugriff auf PORTB
		uint8_t status = SREG;  // SREG sichern
		cli();                  // Interrupts sperren
		PORTB ^= 2;             // LED2 (PB1) toggeln
		SREG = status;          // SREG zurückschreiben ? Interruptstatus wiederherstellen
	}

	return 0;
}

