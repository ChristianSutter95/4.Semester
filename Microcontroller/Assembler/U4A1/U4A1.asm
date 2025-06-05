/*
 * Feb 2021, OST 
 * Subroutine with exact delay of 10us. Processor clock is at 8MHz.
 * Call subroutine 25000 times to get blinking frequency of 2Hz.
 */ 

.INCLUDE "m32U4def.inc"

.ORG		0				// Programmstartadresse im Flash

start:						// Initialisierung	

	// Stackpointer initialisieren (wichtig f�r call/ret/push/pop)
	ldi		r16,HIGH(RAMEND)	// High-Byte von oberstem SRAM
	out		SPH,r16				// in Stack Pointer High schreiben
	ldi		r16,LOW(RAMEND)		// Low-Byte
	out		SPL,r16				// in Stack Pointer Low schreiben

	// Port B vollst�ndig als Ausgang konfigurieren (f�r LED-Ausgabe)
	ldi		r16,0xff
	out		DDRB,r16

loop:	
	ldi		r20,250			// �u�erer Schleifenz�hler
loop1:
	ldi		r21,100			// Innerer Schleifenz�hler
loop2:
	call 	delay10Us		// Subroutine aufrufen (4 Takte)
	subi	r21,1			// inneren Z�hler dekrementieren
	brne	loop2			// zur�ck, solange ? 0
	subi	r20,1			// �u�eren Z�hler dekrementieren
	brne	loop1			// zur�ck, solange ? 0

	// Wenn beide Schleifen fertig ? LED-Zustand toggeln
	in		r17,PINB			// Aktuellen Zustand von PORTB lesen
	eor		r17,r16				// Toggle: XOR mit Maske 0xFF
	out		PORTB,r17			// Neuer Zustand auf PORTB schreiben
	rjmp	loop				// Wiederholen

// Subroutine Delay 10 �s
// Ziel: Gesamtdauer (inkl. call und ret) = genau 80 Takte

delay10Us:                              
       push   r16			// 2 Takte � r16 sichern
       ldi    r16,22		// 1 Takt � Z�hler laden
count:
       subi   r16,1			// 1 Takt
       brne   count			// 2 Takte beim Sprung, 1 beim Beenden

       pop    r16			// 2 Takte � r16 wiederherstellen
       nop					// 1 Takt � Feinanpassung
       nop					// 1 Takt � Feinanpassung
       ret					// 4 Takte � R�cksprung zur Hauptschleife
