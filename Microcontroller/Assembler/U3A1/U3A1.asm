
.INCLUDE "m32U4def.inc"

.ORG 0

start:
    ; --- Variante 1: Direkte Adressierung ---
    ldi     r16, 0xA6        ; Wert, der gespeichert werden soll
    sts     0x0320, r16      ; Speichere r16 direkt an Adresse 0x0320 (dezimal 800)

    ; --- Variante 2: Indirekte Adressierung über X ---
    ldi     r26, low(0x0320) ; X-Low = R26
    ldi     r27, high(0x0320); X-High = R27 ? X = 0x0320
    st      X, r16           ; Speichere r16 an die Adresse, auf die X zeigt

loop:
    rjmp    loop             ; Endlosschleife – Programmende

