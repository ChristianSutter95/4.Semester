/*
 * Feb. 2025, OST 
 * Displays a bit pattern on pins PB0..PB3
 */ 

.INCLUDE "m32U4def.inc"     ; Gerätespezifikationen für ATmega32U4

.ORG    0                   ; Startadresse des Programmcodes

start:
    ldi     r16, 0x0F       ; Binär: 00001111 ? untere 4 Pins als Ausgang konfigurieren
    out     DDRB, r16       ; Setze PORTB[3:0] als Ausgang, PORTB[7:4] bleiben Eingang

    ldi     r16, 0x05       ; Binär: 00000101 ? LED-Muster: Bit 0 und 2 HIGH
    out     PORTB, r16      ; Setze LED-Muster auf Ausgang (PORTB)

loop:
    rjmp    loop            ; Endlosschleife

