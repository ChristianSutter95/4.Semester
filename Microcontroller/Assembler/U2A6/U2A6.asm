/*
 * Feb. 2025, OST 
 * Output on PB1 toggles after three positive edges on PD6.
 */ 

.INCLUDE "m32U4def.inc"     ; Einbinden der Ger�tespezifikationen

.DEF		count=r20       ; Z�hler f�r positive Flanken
.DEF		oldIn=r21       ; Speichert vorherigen Tasterzustand
.DEF		input=r22       ; Speichert aktuellen Tasterzustand

.ORG		0               ; Startadresse des Programms

start:                      
	ldi		count,5         ; Z�hler auf 5 setzen
	in		oldIn,PIND      ; Aktuellen Zustand von PIND einlesen
	sbi		DDRB,1          ; PB1 als Ausgang konfigurieren (f�r LED)
	cbi		DDRD,6          ; PD6 als Eingang konfigurieren (f�r Taster)

loop:	
	in		input,PIND      ; Aktuellen PIND-Zustand lesen
	eor		input,oldIn     ; XOR zur Erkennung einer �nderung an PD6
	sbrs	input,6         ; Wenn Bit 6 *nicht* ge�ndert ? �berspringe n�chste Instruktion
	rjmp	loop            ; Kein Wechsel ? zur�ck zur Schleife

	in		oldIn,PIND      ; Update: neuen Zustand als alten speichern
	sbrs	input,6         ; Pr�fe erneut Bit 6 (neu), ob es 1 ist ? positive Flanke?
	rjmp	loop            ; Wenn nicht 1 ? keine positive Flanke ? zur�ck zur Schleife

	dec		count           ; Positive Flanke erkannt ? Z�hler dekrementieren
	brne	loop            ; Wenn noch nicht 0 ? weiterz�hlen

	ldi		count,5         ; Z�hler zur�cksetzen
	in		r16,PINB        ; Aktuellen LED-Zustand lesen
	ldi		r17,0x02        ; Maske f�r Bit 1 (PB1)
	eor		r16,r17         ; Toggle Bit 1 (XOR mit Maske)
	out		PORTB,r16       ; Schreibe neuen Zustand auf PORTB
	rjmp	loop            ; Endlosschleife
