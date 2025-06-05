/*
 * Feb. 2025, OST 
 * Reads state of button on PD6. When state has changed 5 times the output on PB1 toggles
 */ 

.INCLUDE "m32U4def.inc"     ; Ger�tespezifikation f�r ATmega32U4

.DEF     count = r20        ; Z�hler f�r Pegelwechsel
.DEF     oldIn = r21        ; Vorheriger Zustand von PIND
.DEF     input = r22        ; Aktueller Zustand von PIND

.ORG    0                   ; Startadresse

start:
    ldi     count, 5        ; Initialisiere Z�hler auf 5
    in      oldIn, PIND     ; Lese aktuellen PIND-Zustand
    com     oldIn           ; Invertiere, um Tasterlogik (LOW ? HIGH) zu ber�cksichtigen

    sbi     DDRB, 1         ; Setze PB1 als Ausgang (LED)
    cbi     DDRD, 6         ; Setze PD6 als Eingang (Taster)

loop:
    in      input, PIND     ; Aktuellen Zustand lesen
    com     input           ; Invertiere Tasterzustand (LOW ? HIGH)
    eor     input, oldIn    ; Pr�fe �nderungen durch XOR mit vorherigem Zustand
    sbrs    input, 6        ; Wenn Bit 6 sich nicht ge�ndert hat, Schleife erneut
    rjmp    loop

    in      oldIn, PIND     ; Update: neuen Zustand in oldIn speichern
    com     oldIn           ; Wieder invertieren f�r die n�chste Runde

    dec     count           ; Z�hler verringern
    brne    loop            ; Wenn noch nicht 0, weiter beobachten

    ldi     count, 5        ; Z�hler zur�cksetzen

    in      r16, PINB       ; Aktuellen LED-Zustand lesen
    ldi     r17, 0x02       ; Bitmaske f�r Bit 1 (PB1)
    eor     r16, r17        ; Toggle LED-Zustand
    out     PORTB, r16      ; Schreibe neuen Zustand zur�ck
    rjmp    loop            ; Endlosschleife



