/*
 * March 2021, OST 
 * Blinks LED on PB0 with frequency of 2Hz
 * Uses timer0 interrupt
 * Timer0 runs with 8MHz / 1024 ? 128µs pro Tick
 * Zählt von 178 bis 256 ? 78 Takte ? ~10ms
 * 
 * S1..S3 auf PD4..PD6 werden gelesen (Taster)
 * Sie bilden eine Zahl (0–7), die mit 8 multipliziert wird
 * Diese bestimmt die Pulsdauer für PB1 über Compare Match
 * OCR0A = 178 + (Wert * 8) + 1
 */

.INCLUDE "m32U4def.inc"

.EQU    t0Start = 178        ; Startwert Timer0 für ~10ms
.EQU    cntStart = 25        ; Software-Zähler für 250ms
.EQU    cntAddr  = 0x100     ; Adresse für Software-Zähler im SRAM

.ORG 0                       ; Reset-Vektor
    rjmp start

.ORG OC0Aaddr                ; Compare Match A Interrupt-Vektor
    rjmp T0CMISR

.ORG OVF0addr                ; Timer0 Overflow Interrupt-Vektor
    rjmp T0ISR

.ORG 0x50                    ; Hauptprogrammstart
start:
    ; Stackpointer initialisieren
    ldi r16, HIGH(RAMEND)
    out SPH, r16
    ldi r16, LOW(RAMEND)
    out SPL, r16

    ; PB0, PB1 als Ausgang (LEDs)
    ldi r16, 0x03
    out DDRB, r16

    ; Timer0 starten mit clk/1024
    ldi r16, 5
    out TCCR0B, r16

    ; Timer vorladen (für ersten Überlauf)
    ldi r16, t0Start
    out TCNT0, r16

    ; Software-Zähler für LED-Toggle
    ldi r16, cntStart
    sts cntAddr, r16

    ; Erstinitialisierung von OCR0A
    ldi r16, t0Start + 8      ; Default-Wert (ca. 1 ms Puls)
    out OCR0A, r16

    ; Timer0 Overflow + Compare Match aktivieren
    ldi r16, 3
    sts TIMSK0, r16

    sei                       ; Globale Interrupts freigeben

loop:
    ; Taster (PD4..PD6) lesen
    in    r16, PIND
    andi  r16, 0x70           ; Maske: 0b01110000 ? nur PD4–6

    ; Wert um 4 nach rechts verschieben (PD4 ? Bit0)
    lsr   r16                 ; PD6 ? Bit5, nach Bit4 ...
    lsr   r16
    lsr   r16

    ; Pulsbreite berechnen: OCR0A = t0Start + 1 + (Eingabe * 8)
    ldi   r17, t0Start + 1
    add   r17, r16            ; r17 = OCR0A
    out   OCR0A, r17

    rjmp loop

; ========== Timer0 Overflow ISR (~alle 10 ms) ==========
T0ISR:
    push r16
    push r17
    in   r16, SREG
    push r16

    ldi  r16, t0Start         ; Timer erneut starten
    out  TCNT0, r16

    sbi  PORTB, 1             ; Pulsbeginn: PB1 auf HIGH

    lds  r16, cntAddr
    dec  r16
    sts  cntAddr, r16
    brne restore

    ; Wenn Zähler abgelaufen ? PB0 toggeln
    ldi  r16, cntStart
    sts  cntAddr, r16

    ldi  r16, 0x01
    in   r17, PINB
    eor  r17, r16
    out  PORTB, r17

restore:
    pop  r16
    out  SREG, r16
    pop  r17
    pop  r16
    reti

; ========== Compare Match A ISR ==========
T0CMISR:
    cbi PORTB, 1              ; Pulsende: PB1 auf LOW
    reti
