; for WiNiFe-Compiler
; (cdw) by TAT 2022
;

  .LOCAL

; sichern der aktuellen Adresse
; ___OLD_ADDRESS=*

  *=$A000

; came from line-font.wnf which has to build first
 .include "PACFONT.INC"

; wiederherstellen der aktuellen Adresse
; *=___OLD_ADDRESS

