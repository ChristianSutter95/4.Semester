/*
 * Blinks LED on PB0 mit 2 Hz
 * Verwendet Timer0 Overflow + Compare Match
 * Takt: 8 MHz / 1024 = 128 µs/Tick
 * Zählung von 178 bis 256 = 78 Ticks ? 9.98 ms
 * 
 * Taster S1–S3 (PD4–PD6) ? Zahl (0–7)
 * Zahl * 8 + 1 wird zu OCR0A addiert
 * ? Steuerung der Pulsweite (PWM) auf PB1
 * ? PB1 HIGH ab Overflow, LOW bei Compare Match
 */

#include <avr/io.h>
#include <avr/interrupt.h>

const char T0Start = 178;        // Startwert für Timer0 (~10 ms)
const char counterStart = 25;    // 25 * 10 ms = 250 ms ? 2 Hz
char counter;

// Timer Overflow ISR: alle 10 ms
ISR(TIMER0_OVF_vect) {
	TCNT0 = T0Start;             // Timer zurücksetzen
	PORTB = PINB | 2;            // PB1 HIGH (PWM beginnt)
	counter--;
	if (!counter) {
		counter = counterStart;
		PORTB = PINB ^ 1;        // PB0 toggeln (LED blinkt mit 2 Hz)
	}
}

// Compare Match ISR: nach x Ticks ? PB1 LOW
ISR(TIMER0_COMPA_vect) {
	PORTB = PINB & ~2;           // PB1 auf LOW setzen (PWM-Puls beenden)
}

int main() {
	counter = counterStart;
	DDRB |= 3;                   // PB0 und PB1 als Ausgang
	TCCR0B = 5;                  // Timer0 Prescaler = 1024 ? 128 µs/Tick
	TCNT0 = T0Start;            // Timerzählwert setzen
	TIMSK0 = 3;                  // TOIE0 + OCIE0A (Overflow & Compare Match aktiv)
	OCR0A = T0Start + 8;         // Anfangs-Pulsweite
	sei();                       // Globale Interrupts aktivieren

	while (1) {
		unsigned char in = (PIND & 0x70) >> 1;  // Taster S1–S3 auslesen (Bits 6..4) ? auf Bits 5..3 verschieben
		OCR0A = T0Start + 1 + in;               // PWM-Pulsdauer anpassen (0…7 * 1 Ticks à 128 µs)
	}

	return 0;
}
