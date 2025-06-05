/*
 * Blinks LED on PB0 with frequency of 2Hz
 * Uses timer0 interrupt
 * Timer0 runs with 8 MHz / 1024 ? 128 �s pro Tick
 * Z�hlt von 178 bis 255 ? 78 Ticks * 128 �s ? 9.98 ms
 */

#include <avr/io.h>
#include <avr/interrupt.h>

const char T0Start = 178;          // Startwert f�r Timer (256 - 78)
const char counterStart = 25;      // Anzahl Intervalle � ~10 ms f�r 500 ms
char counter;                      // Interner Z�hler

// Timer0 Overflow Interrupt Service Routine
ISR(TIMER0_OVF_vect) {
	TCNT0 = T0Start;               // Timer zur�cksetzen (neu starten)
	counter--;                     // 10 ms vergangen ? Z�hler dekrementieren
	if (!counter) {
		counter = counterStart;    // zur�cksetzen f�r n�chsten Zyklus
		PORTB = PINB ^ 1;          // PB0 toggeln ? LED blinkt
	}
}

int main() {
	counter = counterStart;        // Startwert f�r 500 ms
	DDRB |= 7;                     // PB0�PB2 als Ausgang
	TCCR0B = 5;                    // Timer0 Prescaler = 1024 ? Takt = 128 �s
	TCNT0 = T0Start;              // Starte mit reduziertem Z�hlwert
	TIMSK0 = 1;                   // Aktiviere Overflow-Interrupt (TOIE0)
	sei();                         // Aktiviere globale Interrupts

	while (1);                     // Leerlauf, Hauptfunktion im Interrupt

	return 0;
}
