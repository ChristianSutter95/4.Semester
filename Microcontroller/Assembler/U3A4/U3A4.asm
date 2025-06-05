/*
 * Feb 2021, OST 
 * Blinks a LED on PB0 with 20Hz. Processor clock is at 8MHz.
 */ 

.INCLUDE "m32U4def.inc"

.EQU    maxCount = 49998        ; 49998 Schleifendurchläufe ? 25 ms bei 8 MHz

.ORG    0                       ; Startadresse im Flash

start:
    ldi     r16, 1              ; Bitmaske für PB0 (LED)
    out     DDRB, r16           ; Setze PB0 als Ausgang ? DDRB = 0b00000001

loop:
    ; --- Delay-Schleife ---
    ldi     ZH, HIGH(maxCount)  ; Lade High-Byte des Zählers ? 1 Takt
    ldi     ZL, LOW(maxCount)   ; Lade Low-Byte des Zählers ? 1 Takt

count:
    sbiw    ZH:ZL, 1            ; Subtrahiere 1 vom Registerpaar r31:r30 ? 2 Takte
    brne    count               ; Wenn nicht 0 ? zurück zu count ? 2 Takte bei Sprung

    ; ? Delay = 49997 Durchläufe × 4 Takte + letzter Durchlauf (3 Takte) = **199991 Takte**

    ; --- LED-Zustand toggeln ---
    in      r17, PINB           ; Lade aktuellen Zustand von PORTB ? 1 Takt
    eor     r17, r16            ; XOR mit Maske (PB0) ? toggelt Bit 0 ? 1 Takt
    out     PORTB, r17          ; Schreibe zurück ? LED ein/aus ? 1 Takt

    ; --- Kleine Pause zur Symmetrie / Timing-Feinabgleich ---
    nop                         ; No Operation ? 1 Takt
    nop                         ; No Operation ? 1 Takt

    rjmp    loop                ; Zurück zum Anfang ? 2 Takte

    ; ? Nach Delay-Schleife: 1 + 1 + 1 + 1 + 1 + 2 = **7 Takte**

