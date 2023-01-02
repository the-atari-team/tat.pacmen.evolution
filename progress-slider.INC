; -*- text -*-
;
; Progress Slider
;
; THIS FILE WILL INCLUDE BY OTHER FILE
;

 .local

?slider_char0=$AFD8+3
?slider_char1=$AFE0+3
?slider_char2=$AFE8+3
?slider_char3=$AFF0+3

?char0=@REG
?char1=@REG+2
?char2=@REG+4
?char3=@REG+6

@clear_progress_slider
  jsr ?init_chars

  ldy #3
  lda #0
?slider_loop
  sta (?char0),y
  sta (?char1),y
  sta (?char2),y
  sta (?char3),y
  dey
  bpl ?slider_loop
  rts

; init char pointer
?init_chars
  lda #<?slider_char0
  sta ?char0
  lda #>?slider_char0
  sta ?char0+1

  lda #<?slider_char1
  sta ?char1
  lda #>?slider_char1
  sta ?char1+1

  lda #<?slider_char2
  sta ?char2
  lda #>?slider_char2
  sta ?char2+1

  lda #<?slider_char3
  sta ?char3
  lda #>?slider_char3
  sta ?char3+1
  rts

; wir setzen einen Progress-Slider, nehmen [0-31], aber alles > 32 ist aber voll
@set_progress_slider
@set_progress_slider_i
  jsr ?init_chars

  ldy #1
  lda (@HEAP_PTR),y

  cmp #31         ; sind wir greater 32
  bcc ?less_31

  lda #31         ; dann auf 32 setzen

?less_31
  asl
  asl
  tax
  stx @REG+8

  ldy #3

?slider_loop2
  ldx @REG+8
  lda ?slider_data,x
  sta (?char0),y
  inx

  lda ?slider_data,x
  sta (?char1),y
  inx

  lda ?slider_data,x
  sta (?char2),y
  inx

  lda ?slider_data,x
  sta (?char3),y
  inx

  dey
  bpl ?slider_loop2
  rts

?slider_data
 .byte ~00000000,~00000000,~00000000,~00000000
 .byte ~00000000,~00000001,~00000000,~00000000
 .byte ~00000000,~00000001,~10000000,~00000000
 .byte ~00000000,~00000011,~10000000,~00000000
 .byte ~00000000,~00000011,~11000000,~00000000
 .byte ~00000000,~00000111,~11000000,~00000000
 .byte ~00000000,~00000111,~11100000,~00000000
 .byte ~00000000,~00000111,~11100000,~00000000 ; add um auf volle 32 zu kommen

 .byte ~00000000,~00001111,~11100000,~00000000
 .byte ~00000000,~00001111,~11110000,~00000000
 .byte ~00000000,~00011111,~11110000,~00000000
 .byte ~00000000,~00011111,~11111000,~00000000
 .byte ~00000000,~00111111,~11111000,~00000000
 .byte ~00000000,~00111111,~11111100,~00000000
 .byte ~00000000,~01111111,~11111100,~00000000
 .byte ~00000000,~01111111,~11111110,~00000000

 .byte ~00000000,~11111111,~11111110,~00000000
 .byte ~00000000,~11111111,~11111111,~00000000
 .byte ~00000001,~11111111,~11111111,~00000000
 .byte ~00000001,~11111111,~11111111,~10000000
 .byte ~00000011,~11111111,~11111111,~10000000
 .byte ~00000011,~11111111,~11111111,~11000000
 .byte ~00000111,~11111111,~11111111,~11000000
 .byte ~00000111,~11111111,~11111111,~11100000

 .byte ~00001111,~11111111,~11111111,~11100000 ; add, um auf volle 32 zu kommen
 .byte ~00001111,~11111111,~11111111,~11100000
 .byte ~00001111,~11111111,~11111111,~11110000
 .byte ~00011111,~11111111,~11111111,~11110000
 .byte ~00011111,~11111111,~11111111,~11111000
 .byte ~00111111,~11111111,~11111111,~11111000
 .byte ~00111111,~11111111,~11111111,~11111100
 .byte ~00111111,~11111111,~11111111,~11111100 ; add, um auf volle 32 zu kommen
