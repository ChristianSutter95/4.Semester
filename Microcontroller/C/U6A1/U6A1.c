/*
 * March 2021, OST 
 * Increments variable up to a certain value
 */ 

#include <avr/io.h> // Bindet alle I/O-Register für den ATmega32U4 ein

int main(void) {
	int a = 0;               // Initialisiere Variable a mit 0

	while (a <= 32)          // Solange a kleiner/gleich 32 ist
		a += 4;              // Erhöhe a um 4

	while (1);              // Endlosschleife, damit das Programm nicht endet
}
