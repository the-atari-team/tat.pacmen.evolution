; for WiNiFe-Compiler
; (cdw) by TAT 2022
;

  .LOCAL

?pmadr=@REG               ; 2 bytes, adresse PM
?positionInCharset=@REG+2 ; 2
?screenaddr=@REG+4        ; 2

?max_copy=@REG+6
?x_pos=@REG+7
?y_pos=@REG+8
?animation_number=@REG+9  ; 1

?animation=@REG+10        ; 2
?pacmenData=@REG+12       ; 2

; Es wird der Pacmen in den screen gezeichnet
; 16 Byte Player Grafix + 2byte oben +2byte unten

@draw_pacmen
  lda #<@PMM
  sta ?pmadr
  lda #>@PMM
  sta ?pmadr+1

  lda @pacmenx
  sta ?x_pos

  clc
  adc #48
  sta hposM3             ; x-position des "Players" here 4 Missiles
  adc #2
  sta HPOSM2
  adc #2
  sta HPOSM1
  adc #2
  sta HPOSM0

  lda @pacmeny
  sta ?y_pos
  clc
  adc #32
  sta ?player_ypos

  lda @pacmen_animation
  sta ?animation
  lda @pacmen_animation+1
  sta ?animation+1

  lda @pacmen_animation_number
  and #127
  asl a
  tay

  jmp ?inner_copy_pacmen

; copypacmen(adr:pm0, x-pos, y-pos, animation, animation_number, player-direction)
@copypacmen
@copypacmen_iiiii

  ldy #1                 ; Adresse der Player/Missiles
  lda (@heap_ptr),y
  lda #0
  sta ?pmadr
  iny
  lda (@heap_ptr),y
  sta ?pmadr+1           ; Anfangsadresse PM-GFX!

  ldy #3                 ; X-Position (0 ist linker Rand)
  lda (@heap_ptr),y
  sta ?x_pos

  clc
  adc #48
  sta hposM3             ; x-position des "Players" here 4 Missiles
  adc #2
  sta HPOSM2
  adc #2
  sta HPOSM1
  adc #2
  sta HPOSM0

  ldy #5                 ; Y-Position
  lda (@heap_ptr),y

  sta ?y_pos
  clc
  adc #32
  sta ?player_ypos

  ldy #7                 ; Pointer zu Animation
  lda (@heap_ptr),y
  sta ?animation
  iny
  lda (@heap_ptr),y
  sta ?animation+1

  ldy #9                 ; Animation Number
  lda (@heap_ptr),y
  and #127               ; max. 127 Animationen moeglich
  asl a
  tay

?inner_copy_pacmen
  lda (?animation),y
  sta ?pacmenData
  iny
  lda (?animation),y
  sta ?pacmenData+1

  sec                    ; wir ziehen von der wirklichen Datenposition
  lda ?pacmenData        ; die ypos ab, so brauchen wir nur Y-Register
  sbc ?player_ypos       ; und koennen X-Register als Zaehler verwenden
  sta ?pacmenData
  lda ?pacmenData+1
  sbc #0
  sta ?pacmenData+1

; zeichnet den Player
?draw_player
  lda #0
  ldy ?player_ypos
  dey                    ; in die obersten 2 Zeilen eine 0
  sta (?pmadr),y
  dey
  sta (?pmadr),y
  iny
  iny

  tya
  clc
  adc #16                ; height of player addieren
  sta ?max_copy

?copy_pm_loop
  lda (?pacmenData),y
  sta (?pmadr),y
  iny
  cpy ?max_copy
  bne ?copy_pm_loop

  lda #0
  sta (?pmadr),y

  rts

?player_ypos
 .byte 0

