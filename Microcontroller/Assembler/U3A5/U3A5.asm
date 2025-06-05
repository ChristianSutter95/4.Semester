/*
 * Feb 2021, OST 
 * Blinks a LED on PB0 with 1Hz. Processor clock is at 8MHz.
 */ 

.INCLUDE "m32U4def.inc"

.EQU    maxCount = 50000      ; Innerer Z�hler: erzeugt ca. 25 ms bei 8 MHz
.EQU    loopCount = 20        ; �u�erer Z�hler: 20 � 25 ms = 500 ms

.ORG    0

start:
    ldi     r16, 1            ; Bitmaske f�r PB0
    out     DDRB, r16         ; PB0 als Ausgang
    ldi     r18, loopCount    ; �u�erer Schleifenz�hler (20)

loop:
    ldi     XH, HIGH(maxCount); Setze inneren Z�hler (50000)
    ldi     XL, LOW(maxCount)

count:
    sbiw    XH:XL, 1          ; Subtrahiere 1 vom inneren Z�hler (2 Takte)
    brne    count             ; Springt zur�ck solange ? 0 (2 Takte bei Sprung)

    dec     r18               ; �u�ere Schleife herunterz�hlen
    brne    loop              ; Wenn r18 ? 0 ? nochmal innerer Loop
                              ; Wenn r18 = 0 ? n�chster Schritt

    ldi     r18, loopCount    ; �u�eren Z�hler zur�cksetzen f�r n�chstes Mal

    in      r17, PINB         ; Lade aktuellen Portzustand (1 Takt)
    eor     r17, r16          ; Toggle Bit 0 ? LED ein/aus (1 Takt)
    out     PORTB, r17        ; Schreibe zur�ck (1 Takt)

    rjmp    start             ; Beginne wieder von vorn


