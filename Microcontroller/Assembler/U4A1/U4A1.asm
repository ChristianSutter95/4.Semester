/*
 * Feb 2021, OST 
 * Subroutine with exact delay of 10us. Processor clock is at 8MHz.
 * Call subroutine 25000 times to get blinking frequency of 2Hz.
 */ 

.INCLUDE "m32U4def.inc"

.ORG		0				// Programmstartadresse im Flash

start:						// Initialisierung	

	// Stackpointer initialisieren (wichtig für call/ret/push/pop)
	ldi		r16,HIGH(RAMEND)	// High-Byte von oberstem SRAM
	out		SPH,r16				// in Stack Pointer High schreiben
	ldi		r16,LOW(RAMEND)		// Low-Byte
	out		SPL,r16				// in Stack Pointer Low schreiben

	// Port B vollständig als Ausgang konfigurieren (für LED-Ausgabe)
	ldi		r16,0xff
	out		DDRB,r16

loop:	
	ldi		r20,250			// Äußerer Schleifenzähler
loop1:
	ldi		r21,100			// Innerer Schleifenzähler
loop2:
	call 	delay10Us		// Subroutine aufrufen (4 Takte)
	subi	r21,1			// inneren Zähler dekrementieren
	brne	loop2			// zurück, solange ? 0
	subi	r20,1			// äußeren Zähler dekrementieren
	brne	loop1			// zurück, solange ? 0

	// Wenn beide Schleifen fertig ? LED-Zustand toggeln
	in		r17,PINB			// Aktuellen Zustand von PORTB lesen
	eor		r17,r16				// Toggle: XOR mit Maske 0xFF
	out		PORTB,r17			// Neuer Zustand auf PORTB schreiben
	rjmp	loop				// Wiederholen

// Subroutine Delay 10 µs
// Ziel: Gesamtdauer (inkl. call und ret) = genau 80 Takte

delay10Us:                              
       push   r16			// 2 Takte – r16 sichern
       ldi    r16,22		// 1 Takt – Zähler laden
count:
       subi   r16,1			// 1 Takt
       brne   count			// 2 Takte beim Sprung, 1 beim Beenden

       pop    r16			// 2 Takte – r16 wiederherstellen
       nop					// 1 Takt – Feinanpassung
       nop					// 1 Takt – Feinanpassung
       ret					// 4 Takte – Rücksprung zur Hauptschleife
