/*
 * Feb. 2025, OST
 * Increment a register up to a certain limit, then stops
 */

.INCLUDE "m32U4def.inc"     ; Bindet die Gerätespezifische Definition für ATmega32U4 ein

.ORG    0                   ; Startadresse des Programmcodes im Flash (Adresse 0)

start:
    ldi r16, 4              ; Lade die Konstante 4 in Register r16
    clr r17                 ; Setze Register r17 auf 0

loop:
    add r17, r16            ; Addiere r16 zu r17 ? r17 = r17 + 4
    cpi r17, 32             ; Vergleiche r17 mit dem Wert 32
    brne loop               ; Falls ungleich (Zero-Flag = 0), springe zurück zu "loop"

end:
    rjmp end                ; Endlosschleife ? bleibt hier stehen



