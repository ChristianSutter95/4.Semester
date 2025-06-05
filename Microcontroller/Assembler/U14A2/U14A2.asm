/*
 * Aufgabe 2: 8�8-Bit Multiplikation mit Ergebnis in 16 Bit
 * - Multiplikand X: r16
 * - Multiplikator Y: r18
 * - Produkt P = X * Y ? Ergebnis in r17:r16
 * - Nur vorzeichenlose Multiplikation
 */

.INCLUDE "m16U4def.inc"  ; Definitionsdatei f�r ATmega16U4

.ORG 0
start:
	; === Stackpointer initialisieren ===
	ldi  r16, HIGH(RAMEND)
	out  SPH, r16
	ldi  r16, LOW(RAMEND)
	out  SPL, r16

	; === Operanden setzen ===
	ldi  r16, 3       ; Multiplikand (X) in r16
	clr  r17          ; High-Byte des Produkts auf 0 setzen
	ldi  r18, 0xFF    ; Multiplikator (Y) in r18

	call mult16       ; Subroutine aufrufen

loop:
	rjmp loop         ; Endlosschleife (zum Debuggen)

; === Subroutine: mult16 ===
; Multipliziert r16 * r18 ? Ergebnis in r17:r16
mult16:
	push r19              ; r19 sichern (wird als Schleifenz�hler verwendet)
	ldi  r19, 8           ; 8 Bits f�r 8 Durchl�ufe

next:
	clc                   ; Carry-Flag l�schen (wichtig vor add)
	sbrc r16, 0           ; Wenn Bit 0 von r16 gesetzt: n�chsten Befehl ausf�hren
	add  r17, r18         ; Teilprodukt addieren (gewichtetes Y)

	ror  r17              ; Carry ? MSB von r17
	ror  r16              ; Carry ? MSB von r16 (Shift rechts Gesamtprodukt)

	dec  r19              ; Z�hler --
	brne next             ; Wiederholen bis 8 Durchl�ufe abgeschlossen

	pop  r19              ; Urspr�nglichen Wert von r19 wiederherstellen
	ret

