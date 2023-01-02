; Kontrolle der Kollision

; liefert 0, wenn keine Kollision zwischen Player und Missile stattgefunden hat, sonst einen Wert != 0
@Pacmen_collision_with_ghost
  lda SIZEP   ; 4
  ora SIZEP+1 ; 4
  ora SIZEP+2 ; 4
  ora SIZEP+3 ; 4
  tay         ; 2
  ldx #0      ; 2
  rts         ; 6 sum 26 + jsr ;6 => 32 cycles
