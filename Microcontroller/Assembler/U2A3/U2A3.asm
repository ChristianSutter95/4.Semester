/*
 * Feb. 2025, OST 
 * Reads states of buttons on PD4..PD7 and outputs this state on PB0..PB3
 */ 

.INCLUDE "m32U4def.inc"     ; Gerätespezifikation für ATmega32U4

.ORG    0                   ; Startadresse

start:
    ldi     r16, 0x0F       ; 00001111 ? PORTB[3:0] als Ausgang konfigurieren
    out     DDRB, r16       ; LEDs an PORTB.0 bis PORTB.3

    clr     r16             ; r16 = 0
    out     DDRD, r16       ; PORTD komplett als Eingang (für Taster)

loop:
    in      r16, PIND       ; Tasterzustände einlesen (PORTD)
    com     r16             ; Invertiere Bits (LOW ? HIGH, HIGH ? LOW)
    swap    r16             ; Tausch oberes und unteres Nibble
    out     PORTB, r16      ; Gib Zustand auf PORTB.3:0 aus
    rjmp    loop            ; Endlosschleife


