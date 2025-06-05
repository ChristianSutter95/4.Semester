/*
 * March 2021, OST 
 * Blinks LED on PB0 with frequency of 2Hz
 * Uses timer0 interrupt
 * Timer0 runs with 8Mhz / 1024 -> 128us per tick
 * counts from 178 to 256 -> 78 * 128us = 9.98ms
 */ 
 
.INCLUDE "m32U4def.inc"

.EQU    t0Start = 178        ; Vorladewert ? 256 - 78 ? ergibt ~10 ms bei clk/1024
.EQU    cntStart = 25        ; LED wird nach 25 Überläufen getoggelt ? 250 ms
.EQU    cntAddr = 0x100      ; Speicheradresse für Software-Zähler im SRAM

.ORG 0                       ; Reset-Vektor (Sprung zu Start)
    rjmp start

.ORG OVF0addr                ; Timer0 Overflow Interrupt-Vektor (Adresse = 0x0020)
    rjmp T0ISR               ; Sprung zur ISR

.ORG 0x50                    ; Hauptprogrammstart
start:
    ; Stackpointer initialisieren (RAMEND ? SP)
    ldi r16, HIGH(RAMEND)
    out SPH, r16
    ldi r16, LOW(RAMEND)
    out SPL, r16

    ; PORTB.0 als Ausgang konfigurieren (LED)
    ldi r16, 0x01
    out DDRB, r16

    ; Timer0 konfigurieren: clk/1024 ? Prescaler einstellen
    ldi r16, 5               ; CS02:0 = 101 ? clk/1024
    out TCCR0B, r16

    ; Timer0 mit Vorladewert starten (178)
    ldi r16, t0Start
    out TCNT0, r16

    ; Timer Overflow Interrupt aktivieren (TOIE0)
    ldi r16, 1
    sts TIMSK0, r16

    ; Software-Zähler initialisieren (25 Interrupts = 250 ms)
    ldi r16, cntStart
    sts cntAddr, r16

    ; Globale Interrupts aktivieren
    sei

loop:
    rjmp loop                ; Endlosschleife – alles passiert in ISR

; ===== Interrupt Service Routine (alle 10 ms ausgelöst) =====
T0ISR:
    ; Kontext sichern (r16, r17, SREG)
    push r16
    push r17
    in   r16, SREG
    push r16

    ; Timer erneut vorladen ? damit wieder in 78 Taktzyklen überläuft
    ldi  r16, t0Start
    out  TCNT0, r16

    ; Software-Zähler laden, dekrementieren und zurückschreiben
    lds  r16, cntAddr
    dec  r16
    sts  cntAddr, r16
    brne restore             ; wenn ? 0 ? Ende der ISR

    ; Wenn Zähler = 0 ? LED toggeln und Zähler neu laden
    ldi  r16, cntStart
    sts  cntAddr, r16
    ldi  r16, 0x01
    in   r17, PINB
    eor  r17, r16
    out  PORTB, r17

restore:
    ; Kontext wiederherstellen (SREG, r17, r16)
    pop  r16
    out  SREG, r16
    pop  r17
    pop  r16
    reti                     ; Rücksprung aus ISR

