/*
 * Aufgabe 1: 16-Bit Addition in Assembler
 * - Zwei Operanden (16 Bit) werden addiert
 * - Operand 1: r16 (high), r17 (low)
 * - Operand 2: r18 (high), r19 (low)
 * - Ergebnis: zurück in r16:r17
 */

.INCLUDE "m32U4def.inc"   ; AVR-spezifische Registerdefinitionen

.ORG 0                    ; Reset-Vektor-Adresse
start:
	; === Stackpointer initialisieren ===
	ldi  r16, HIGH(RAMEND)
	out  SPH, r16
	ldi  r16, LOW(RAMEND)
	out  SPL, r16

	; === Testdaten laden ===
	ldi  r16, 0x22         ; Summand 1 (high)
	ldi  r17, 0xFF         ; Summand 1 (low)
	ldi  r18, 0x44         ; Summand 2 (high)
	ldi  r19, 0x02         ; Summand 2 (low)

	call add16            ; Subroutine aufrufen (Standardversion)

loop:
	rjmp loop             ; Endlosschleife (Simulation/Test)

; === Subroutine: add16 ===
; Führt 16-Bit-Addition durch mit manuellem Übertrags-Handling
add16:
	add  r17, r19         ; LSB + LSB
	brcc next             ; Wenn kein Übertrag ? weiter
	inc  r16              ; Wenn Carry ? HSB um 1 erhöhen
next:
	add  r16, r18         ; MSB + MSB (ohne Carry)
	ret

; === Subroutine: add16fast ===
; Optimierte Variante mit 'adc' (add with carry)
add16fast:
	add  r17, r19         ; LSB + LSB
	adc  r16, r18         ; MSB + MSB + evtl. Carry aus vorher
	ret

