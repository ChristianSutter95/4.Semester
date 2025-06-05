/*
 * Feb. 2025, OST 
 * Decrement a memory location by three, stop when value is below -8
 */ 

.INCLUDE "m32U4def.inc"      ; Einbinden der Gerätespezifikation für ATmega32U4

.EQU var_a = 0x100           ; Definiert eine symbolische Adresse im SRAM (erste freie Adresse)

.ORG    0                    ; Startadresse im Flash

start:
    ldi     r16, 3           ; r16 = 3 (Abzug)
    ldi     r17, 5           ; r17 = Startwert
    sts     var_a, r17       ; Speicherstelle var_a = 5

loop:
    lds     r17, var_a       ; Lade aktuellen Wert von var_a in r17
    sub     r17, r16         ; Ziehe 3 ab
    sts     var_a, r17       ; Speichere Ergebnis zurück in var_a
    cpi     r17, -8          ; Vergleiche mit -8 (entspricht 0xF8)
    brpl    loop             ; Springe zurück, wenn Ergebnis ? 0
end:
    rjmp    end              ; Endlosschleife

