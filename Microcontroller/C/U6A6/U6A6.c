/*
 * March 2021, OST 
 * flashes a led with a delay of approximately 250ms
 */ 

#include <avr/io.h> // Zugriff auf Portregister

// Softwareverzögerung: 2 geschachtelte Schleifen erzeugen Wartezeit
void delay250ms() {
	for (int i = 0; i < 100; i++)
		for (int k = 0; k < 1000; k++); // Leere innere Schleife
}

int main() {

	DDRB |= 0x0f;               // Setzt PB0–PB3 als Ausgang (z.?B. LEDs)

	while (1) {
		delay250ms();          // Warten (ca. 250?ms)
		PORTB = PINB ^ 0x0f;   // Toggle Bits 0–3 von Port B (LEDs wechseln Zustand)
	}

	return 0;
}
