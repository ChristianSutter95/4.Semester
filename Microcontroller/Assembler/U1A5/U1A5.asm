/*
 * Feb. 2025, OST 
 * Clears bits in register
 */ 

.INCLUDE "m32U4def.inc"     ; Gerätespezifikation für ATmega32U4

.ORG    0                   ; Startadresse des Programms im Flash

start:
    ldi     r16, 15         ; Lade Dezimalwert 15 (0x0F = 00001111b) in r16

loop:
    com     r16             ; Komplemetiere alle Bits in r16 ? Bitweise Invertierung (1?0)
    rjmp    loop            ; Endlosschleife – kehrt immer wieder zur Invertierung zurück

