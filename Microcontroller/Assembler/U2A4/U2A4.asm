/*
 * Feb. 2025, OST 
 * Reads state of button on PD6 and outputs state on PB1
 */ 

.INCLUDE "m32U4def.inc"     ; Gerätespezifikationen

.ORG    0                   ; Programmstart

start:
    sbi     DDRB, 1         ; Setze PORTB.1 als Ausgang (LED)
    cbi     DDRD, 6         ; Setze PORTD.6 als Eingang (Taster)

loop:
    in      r16, PIND       ; Lade PortD (Eingänge)
    com     r16             ; Invertiere Tasterzustände (aktiv-LOW zu aktiv-HIGH)
    bst     r16, 6          ; Kopiere Bit 6 ins T-Flag
    clr     r16             ; Setze r16 auf 0
    bld     r16, 1          ; Lade T-Flag in Bit 1 von r16
    out     PORTB, r16      ; Setze LED entsprechend am PB1
    rjmp    loop            ; Endlosschleife



