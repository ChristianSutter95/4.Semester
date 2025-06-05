/*
 * Jan 2021, OST 
 * Clears bits in register
 */ 

.INCLUDE "m32U4def.inc"     ; Einbinden der Ger�tespezifikationen f�r ATmega32U4

.ORG    0                   ; Programmstart bei Adresse 0

start:
    ldi     r16, 0xFF       ; Lade 11111111 ? alle Bits = 1

loop:
    lsr     r16             ; Logical Shift Right: schiebt alle Bits nach rechts, Bit 0 ? Carry, Bit 7 = 0
    sbrc    r16, 0          ; Skip if Bit in Register is Cleared: �berspringt n�chste Instruktion, wenn Bit 0 = 0
    brne    loop            ; Wenn Zero-Flag nicht gesetzt (also r16 != 0), gehe zu loop
    rjmp    start           ; Wenn alle Bits gel�scht sind (r16 = 0), Neustart

