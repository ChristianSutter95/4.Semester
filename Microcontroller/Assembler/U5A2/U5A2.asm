/*
 * March 2021, OST 
 * Blinks LED on PB0 with frequency of 2Hz
 * Uses timer0 interrupt
 * Timer0 runs with 8Mhz / 1024 -> 128us per tick
 * Counts from 178 to 256 -> 78 * 128us = 9.98ms
 * 
 * OCR0A = 178 + 9 -> 8 * 128us = 1.024ms
 * PB1 is set every 10ms, and reset when a match occurs 
 */ 
 
.INCLUDE "m32U4def.inc"

.EQU    t0Start  = 178        ; Startwert für Timer (256 - 78)
.EQU    cntStart = 25         ; Software-Zähler (25 × 10 ms = 250 ms)
.EQU    cntAddr  = 0x100      ; Speicheradresse für Zähler im SRAM

.ORG 0                        ; Reset-Vektor
    rjmp start

.ORG OC0Aaddr                 ; Compare Match A Interrupt-Vektor
    rjmp T0CMISR

.ORG OVF0addr                 ; Overflow Interrupt-Vektor von Timer0
    rjmp T0ISR

.ORG 0x50                     ; Hauptprogrammstart
start:
    ; Stackpointer initialisieren
    ldi r16, HIGH(RAMEND)
    out SPH, r16
    ldi r16, LOW(RAMEND)
    out SPL, r16

    ; PB0 und PB1 als Ausgänge (LEDs)
    ldi r16, 0x03
    out DDRB, r16

    ; Timer0 konfigurieren: Prescaler clk/1024
    ldi r16, 5
    out TCCR0B, r16

    ; Timer0 vorladen für 10 ms-Überlauf
    ldi r16, t0Start
    out TCNT0, r16

    ; Software-Zähler setzen (für PB0-Toggle)
    ldi r16, cntStart
    sts cntAddr, r16

    ; Compare-Match bei t0Start + 9 = 187 ? ~1 ms nach Start
    ldi r16, t0Start + 9
    out OCR0A, r16

    ; Enable Compare Match A + Overflow Interrupts
    ldi r16, 3               ; Bit 0 (TOIE0), Bit 1 (OCIE0A)
    sts TIMSK0, r16

    sei                      ; Globale Interrupts aktivieren

loop:
    rjmp loop                ; Hauptprogramm bleibt in Endlosschleife

; ===== Timer0 Overflow Interrupt (alle ~10 ms) =====
T0ISR:
    push r16
    push r17
    in   r16, SREG
    push r16

    ; Timer erneut starten (für exakten Zyklus)
    ldi  r16, t0Start
    out  TCNT0, r16

    ; PB1 = HIGH (1 ms Puls beginnt hier)
    sbi  PORTB, 1

    ; Software-Zähler zählen (für 250 ms)
    lds  r16, cntAddr
    dec  r16
    sts  cntAddr, r16
    brne restore             ; Wenn Zähler ? 0 ? zurück

    ; PB0 toggeln (Ende 250 ms)
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
    reti                     ; Rücksprung aus ISR

; ===== Compare Match A Interrupt (ca. 1 ms nach Overflow) =====
T0CMISR:
    cbi PORTB, 1             ; PB1 = LOW ? 1 ms Puls ist vorbei
    reti
