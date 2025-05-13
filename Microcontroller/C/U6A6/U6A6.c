/*
 * March 2021, OST 
 * flashes a led with a delay of approximately 250ms
 */ 

#include <avr/io.h>

// delay, duration depends on compiler optimization level, choose none
void delay250ms() {
	for (int i = 0; i < 100; i++)
		for (int k = 0; k < 1000; k++); 
}

int main() {

	DDRB |= 0x0f;

	while (1) {
		delay250ms();
		PORTB = PINB ^ 0x0f;
	}

	return 0;
}