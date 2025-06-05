;
; AssemblerProject_Template.asm
;
; Created: 13.05.2025 08:58:53
; Author : chris
;


.INCLUDE "m32U4def.inc"

.EQU    maxCount = 50000         ; innerer Schleifenzähler
.EQU    loopCount = 20           ; äußerer Schleifenzähler

.ORG 0

start:
    ; Stack initialisieren (SPH:SPL = RAMEND)
    ldi r16, HIGH(RAMEND)
    out SPH, r16
    ldi r16, LOW(RAMEND)
    out SPL, r16

    ; Port B0 als Ausgang setzen (LED-Pin)
    ldi r16, 0xFF          ; Maske: nur Bit 0 = 1
    out DDRB, r16               ; DDRB.0 = Ausgang

    ; OSCCAL (Taktkalibrierung) ändern – optional
    ; ? höhere Werte = schnellerer Takt
    ; ? niedrigere Werte = langsamerer Takt
    ; ldi r16, 0xAF             ; Beispielwert (manuell aktivieren)
    ; out OSCCAL, r16

loop:
    ; Äußerer Schleifenzähler initialisieren
    ldi r18, loopCount          ; 20 Wiederholungen

outer:
    ; Inneren Schleifenzähler (50000)
    ldi XH, HIGH(maxCount)
    ldi XL, LOW(maxCount)

inner:
    sbiw XH:XL, 1               ; Z = Z - 1 ? 2 Takte
    brne inner                  ; Wenn ? 0 ? weiter zählen ? 2 Takte

    dec r18                     ; Äußere Schleife runterzählen
    brne outer                  ; Wenn ? 0 ? nochmal innere Schleife

    ; LED toggeln über XOR
    in r17, PINB                ; Aktuellen Zustand von PORTB lesen
    eor r17, r16                ; Bit 0 toggeln (XOR mit 0b00000001)
    out PORTB, r17              ; Neuen Zustand ausgeben

    rjmp loop                   ; Wiederhole alles

