/*
 * Feb 2021, OST 
 * Subroutine which toggles memory location.
 */ 

.INCLUDE "m32U4def.inc"

.EQU    val  = 0x100        ; Speicheradresse für den Wert
.EQU    mask = 0xf0         ; Maske für Toggle-Operation

.ORG    0                   ; Startadresse im Flash

start:
    ; Stackpointer initialisieren (SPH:SPL = RAMEND)
    ldi     r16, HIGH(RAMEND)
    out     SPH, r16
    ldi     r16, LOW(RAMEND)
    out     SPL, r16

    ; Starte mit Wert 0xAA an Adresse 0x0100
    ldi     r16, 0xAA
    sts     val, r16

    ; Lade Maske 0xF0 in r20 (wird später für XOR verwendet)
    ldi     r20, mask

loop:
    rcall   toggle           ; Rufe Subroutine zum XOR-Toggle auf
    rjmp    loop             ; Endlosschleife

; ===== Subroutine: toggle =====
; Führt: Speicherwert ^= Maske (XOR)
; Universell nutzbar durch Stack-konformes Sichern von r16

toggle:
    push    r16              ; Register sichern
    lds     r16, val         ; Wert aus Adresse 0x0100 laden
    eor     r16, r20         ; XOR mit Maske (r20 = 0xF0)
    sts     val, r16         ; Ergebnis zurückschreiben
    pop     r16              ; r16 wiederherstellen
    ret                      ; Rücksprung
