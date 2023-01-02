; for WiNiFe-Compiler
; (cdw) by TAT 2022
;

  .LOCAL

; sichern der aktuellen Adresse
FONT_OLD_ADDRESS=*

  *=$AE00 ; - $B000 fuer Graphics 1
 .include "TITLFONT.INC"

; $B000 - $B300 (Wir koennen nur 96 Zeichen nutzen)
; Enthaelt
; - Spielfeld
; - Augen
; - Ein paar Collectibles (Kirsche, ...)
; - E=X=T=R=A= font
;

; Dieser Font sollte entpackt werden koennen
 .include "PACFONT.INC" ; playfield font


  *=$B800 ; - $BBA0 -1 (928 Bytes!) Zeichen 0 - 115
; Enthaelt:
; - Grosse Buchstaben
; - Zahlen
; - PACMEN Title (10 Bytes 8x8 ist ein Buchstabe, A ist nicht doppelt!)
; - evolution Title + Sterne (16 Bytes)
; 62 bis 74 sind noch frei
; TODO: ist nicht ganz optimiert.
 .include "PACFONT2.INC" ; pacmen

; wiederherstellen der aktuellen Adresse
  *=FONT_OLD_ADDRESS

