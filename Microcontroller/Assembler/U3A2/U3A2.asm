/*
 * Feb 2021, OST 
 * Fills memory locations from address 800 to 850.
 */ 

.INCLUDE "m32U4def.inc"

.EQU    adr = 800          ; Startadresse = 0x0320 im SRAM

.ORG    0                  ; Startadresse im Flash

start:
    ldi     XL, LOW(adr)   ; X-Register Low-Byte ? r26
    ldi     XH, HIGH(adr)  ; X-Register High-Byte ? r27 ? X = 0x0320

    ldi     r16, 10        ; Startwert = 10
    ldi     r17, 3         ; Inkrement = 3
    ldi     r18, 50        ; 50 Werte schreiben ? bis 0x0351 (einschließlich)

loop:
    st      X+, r16        ; Speichere r16 an Adresse X, X += 1
    add     r16, r17       ; Erhöhe r16 um 3
    dec     r18            ; Zähle ab
    brne    loop           ; Wenn noch nicht 0 ? weiter

end:
    rjmp    end            ; Endlosschleife
