/*
 * Aufgabe 3: Floating-Point Berechnungen und Zyklusdauer
 * 
 * - Demonstriert die Rechenkosten g�ngiger Funktionen (sin, exp, pow)
 * - Ger�t: ATmega16 (kein Hardware-FPU)
 * - Umgebung: Simulator mit "Cycle Counter" in der "Processor View"
 */

#include <math.h>  // Enth�lt Deklarationen f�r sin(), exp(), pow()

int main() {
	float f1 = 0.1, f2;

	f2 = sin(f1);      // Sinusfunktion, ben�tigt ca. 1726 Zyklen
	f2 = exp(f1);      // Exponentialfunktion, ca. 2565 Zyklen
	f2 = pow(f1, 3.4); // Potenzieren, ca. 4945 Zyklen (besonders teuer)

	return 0;
}
