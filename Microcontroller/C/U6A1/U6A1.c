/*
 * March 2021, OST 
 * Increments variable up to a certain value
 */ 

#include <avr/io.h>


int main(void) {
	int a = 0;
	while (a <= 32) a += 4;
	while (1);
}

