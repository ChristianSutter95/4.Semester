/*
 * Jan 2021, OST 
 * Toggles bits in register
 */ 

.INCLUDE "m32U4def.inc"     ; Ger�tespezifikationen f�r ATmega32U4 einbinden

.ORG    0                   ; Startadresse im Flash-Speicher

start:
    ldi     r16, 0xFF       ; Lade Register r16 mit 0b11111111
    clc                     ; L�scht das Carry-Flag im Statusregister

loop:
    ror     r16             ; Rotate Right durch Carry: Bit 0 ? C, C ? Bit 7
    sbrc    r16, 0          ; �berspringe n�chste Instruktion, falls Bit 0 = 0
    brne    loop            ; Springe zu loop, wenn r16 ? 0
    rjmp    start           ; Wenn r16 = 0, Neustart (Register wieder auf 0xFF setzen)

