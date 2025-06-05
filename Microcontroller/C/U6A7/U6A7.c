/*
 * Blinks LED on PB0 with frequency of 2Hz
 * Uses timer0 interrupt
 * Timer0 runs with 8 MHz / 1024 ? 128 µs pro Tick
 * Zählt von 178 bis 255 ? 78 Ticks * 128 µs ? 9.98 ms
 */

#include <avr/io.h>
#include <avr/interrupt.h>

const char T0Start = 178;          // Startwert für Timer (256 - 78)
const char counterStart = 25;      // Anzahl Intervalle à ~10 ms für 500 ms
char counter;                      // Interner Zähler

// Timer0 Overflow Interrupt Service Routine
ISR(TIMER0_OVF_vect) {
	TCNT0 = T0Start;               // Timer zurücksetzen (neu starten)
	counter--;                     // 10 ms vergangen ? Zähler dekrementieren
	if (!counter) {
		counter = counterStart;    // zurücksetzen für nächsten Zyklus
		PORTB = PINB ^ 1;          // PB0 toggeln ? LED blinkt
	}
}

int main() {
	counter = counterStart;        // Startwert für 500 ms
	DDRB |= 7;                     // PB0–PB2 als Ausgang
	TCCR0B = 5;                    // Timer0 Prescaler = 1024 ? Takt = 128 µs
	TCNT0 = T0Start;              // Starte mit reduziertem Zählwert
	TIMSK0 = 1;                   // Aktiviere Overflow-Interrupt (TOIE0)
	sei();                         // Aktiviere globale Interrupts

	while (1);                     // Leerlauf, Hauptfunktion im Interrupt

	return 0;
}
