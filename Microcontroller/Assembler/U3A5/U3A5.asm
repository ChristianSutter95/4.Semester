/*
 * Feb 2021, OST 
 * Blinks a LED on PB0 with 1Hz. Processor clock is at 8MHz.
 */ 

.INCLUDE "m32U4def.inc"

.EQU    maxCount = 50000      ; Innerer Zähler: erzeugt ca. 25 ms bei 8 MHz
.EQU    loopCount = 20        ; Äußerer Zähler: 20 × 25 ms = 500 ms

.ORG    0

start:
    ldi     r16, 1            ; Bitmaske für PB0
    out     DDRB, r16         ; PB0 als Ausgang
    ldi     r18, loopCount    ; äußerer Schleifenzähler (20)

loop:
    ldi     XH, HIGH(maxCount); Setze inneren Zähler (50000)
    ldi     XL, LOW(maxCount)

count:
    sbiw    XH:XL, 1          ; Subtrahiere 1 vom inneren Zähler (2 Takte)
    brne    count             ; Springt zurück solange ? 0 (2 Takte bei Sprung)

    dec     r18               ; Äußere Schleife herunterzählen
    brne    loop              ; Wenn r18 ? 0 ? nochmal innerer Loop
                              ; Wenn r18 = 0 ? nächster Schritt

    ldi     r18, loopCount    ; Äußeren Zähler zurücksetzen für nächstes Mal

    in      r17, PINB         ; Lade aktuellen Portzustand (1 Takt)
    eor     r17, r16          ; Toggle Bit 0 ? LED ein/aus (1 Takt)
    out     PORTB, r17        ; Schreibe zurück (1 Takt)

    rjmp    start             ; Beginne wieder von vorn


