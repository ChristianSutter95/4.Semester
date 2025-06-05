/*
 * Jan 2021, OST 
 * Decrement a register by three, stop when value is below -8
 */ 

.INCLUDE "m32U4def.inc"     ; Geräteabhängige Definition für ATmega32U4

.ORG    0                   ; Startadresse des Programmcodes im Flash

start:                      ; Initialisierung
    ldi     r16, 3          ; Lade 3 in r16 (Subtraktionswert)
    ldi     r17, 5          ; Lade 5 in r17 (Startwert)

loop:
    sub     r17, r16        ; Subtrahiere r16 von r17 ? r17 = r17 - 3
    cpi     r17, -8         ; Vergleiche r17 mit -8 (Zweierkomplement = 0xF8)
    brpl    loop            ; Wenn Ergebnis ? 0 (Positiv), Schleife fortsetzen

end:
    rjmp    end             ; Endlosschleife nach Schleifenende

