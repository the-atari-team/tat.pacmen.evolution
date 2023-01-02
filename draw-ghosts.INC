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
?ghostData=@REG+12        ; 2

?eye_char=@REG+14         ; 1
?byte_position_in_line=@REG+15

?ghost_x_pos=133

; Es wird ein Geist in den Screen gezeichnet
; 16 Byte Player Grafix
; Dazu wird berechnet, an welcher Position die Augen im Zeichensatz liegen
; Die Augen benoetigen 2-4 Bytes und die 8-32 Bytes im Zeichensatz
; 2 Bytes, wenn x mod 4 == 0 und y mod 8 == 0
; 3 Bytes, wenn x nicht durch 4 teilbar ist und y mod 8 == 0
; 4 Bytes, wenn x an einer / 4 teilbaren Position liegen, aber y an einer nicht / 8 teilbaren Position

; Macht zusammen 16 Zeichen fuer die Augen oder 128 Bytes
 .macro bgcolor
 lda #%1
 sta colbk
 .endm

;@init_copyghost
;  lda #0
;  sta ?last_pupils_direction
;  sta ?last_pupils_direction+1
;  sta ?last_pupils_direction+2
;  sta ?last_pupils_direction+3
;  rts
;
;?last_pupils_direction
; .byte 0,0,0,0

?pm0_high
 .byte $b4, $b5, $b6, $b7

; Das hier wurde abgeloest, da wirklich sehr teuer
;begin
;  x := ghostx[number]
;  y := ghosty[number]
;  animation := ghost_animation[number]
;  animation_number := ghost_animation_number[number]
;  direction := ghost_direction[number] // 0 (left) 1 (right)
;  move_direction := ghost_move_direction[number] // left, right, up, down
;  pupils := ghost_pupils[number]
;  @copyghost(pmstart, number, x, y, animation, animation_number, direction, pupils)
;end
@draw_ghost ;(number)
@draw_ghost_i
;  lda #$34
;  sta colbk

  ldy #1
  lda (@heap_ptr),y
  and #3
  sta ?player
  tax

  lda #0
  sta ?pmadr
  lda ?pm0_high,x
  sta ?pmadr+1

  lda #0
  sta ?eye_char+1

  lda gh_ghost_pupils_buffer,x

  sta ?eye_char
  asl                    ; Zeichen * 8 + 256 * CHBAS
  asl
  rol ?eye_char+1
  asl
  rol ?eye_char+1
  sta ?positionInCharset
  clc
  lda @drawableChbas
  adc ?eye_char+1
  sta ?positionInCharset+1

  lda @ghost_visible,x
  beq ?unsichtbar

  lda @ghostx,x
  sta ?x_pos

  cmp #153
  bcc ?sichtbar2

?unsichtbar
  lda #0
  sta ?ghost_x_pos,x
  sta hposp,x
  rts

?sichtbar2
  and #3
  sta ?position_in_char  ; X-Position innerhalb des Font 0,1,2,3 moeglich
  lda ?x_pos
  lsr
  lsr
  sta ?byte_position_in_line     ; X-Position innerhalb Screen

  lda @ghosty,x
  sta ?y_pos
  clc
  adc #32
  sta ?player_ypos

  jsr ?calc_screenaddr

  lda @ghost_animation_low,x
  sta ?animation
  lda @ghost_animation_high,x
  sta ?animation+1

  lda @ghost_animation_number,x
  and #127
  asl a
  tay

  lda (?animation),y
  sta ?ghostData
  iny
  lda (?animation),y
  sta ?ghostData+1

  sec                    ; wir ziehen von der wirklichen Datenposition
  lda ?ghostData         ; die ypos ab, so brauchen wir nur Y-Register
  sbc ?player_ypos       ; und koennen X-Register als Zaehler verwenden
  sta ?ghostData
  lda ?ghostData+1
  sbc #0
  sta ?ghostData+1

  lda @ghost_direction,x ; 0 (left) 1 (right)
  and #1
  sta ?player_direction

  lda @ghost_pupils,x
  sta ?pupils_direction

  jmp ?inner_copy_ghost


; @deprecated
; Es sollte direkt @draw_ghost(number) verwendet werden
;
;
; copyghost(adr:pm0,
;           number player,
;           x-pos, y-pos,
;           ghost-animation,
;           ghost-animation-number,
;           player-direction,
;           pupils-direction)
;@copyghost
;@copyghost_iiiiiiii
;
;  ldy #1                 ; Adresse der Player/Missiles
;  lda (@heap_ptr),y
;  lda #0
;  sta ?pmadr
;  iny
;  lda (@heap_ptr),y
;  sta ?pmadr+1           ; Anfangsadresse PM-GFX!
;
;
;  ldy #3                 ; Nummer des Players
;  lda (@heap_ptr),y
;  and #3
;  sta ?player
;  tax
;
;  clc
;  adc #4                 ; add 1k
;  adc ?pmadr+1           ; add 256 * player number
;  sta ?pmadr+1           ; in (?pmadr) sollte jetzt richtig in den Player zeigen
;
;  lda #0
;  sta ?eye_char+1
;
;  lda gh_ghost_pupils_buffer,x ; Characterset-Start der Augenpaare
;
;  sta ?eye_char
;  asl                    ; Zeichen * 8 + 256 * CHBAS
;  asl
;  rol ?eye_char+1
;  asl
;  rol ?eye_char+1
;  sta ?positionInCharset
;  clc
;  lda @drawableChbas
;  adc ?eye_char+1
;  sta ?positionInCharset+1
;
;
;  ldy #5                 ; X-Position (0 ist linker Rand)
;  lda (@heap_ptr),y
;  sta ?x_pos
;
;  cmp #153
;  bcc ?sichtbar
;
;  jmp ?unsichtbar

;?sichtbar
;  and #3
;  sta ?position_in_char  ; X-Position innerhalb des Font 0,1,2,3 moeglich
;  lda ?x_pos
;  lsr
;  lsr
;  sta ?byte_position_in_line     ; X-Position innerhalb Screen
;
;
;  ldy #7                 ; Y-Position
;  lda (@heap_ptr),y
;
;  sta ?y_pos
;  clc
;  adc #32
;  sta ?player_ypos
;
;  jsr ?calc_screenaddr
;
;
;  ldy #9                 ; Pointer zu Animation
;  lda (@heap_ptr),y
;  sta ?animation
;  iny
;  lda (@heap_ptr),y
;  sta ?animation+1
;
;
;  ldy #11                ; Animation number
;  lda (@heap_ptr),y
;  and #127
;  asl a
;  tay
;
;  lda (?animation),y
;  sta ?ghostData
;  iny
;  lda (?animation),y
;  sta ?ghostData+1
;
;  sec                    ; wir ziehen von der wirklichen Datenposition
;  lda ?ghostData         ; die ypos ab, so brauchen wir nur Y-Register
;  sbc ?player_ypos       ; und koennen X-Register als Zaehler verwenden
;  sta ?ghostData
;  lda ?ghostData+1
;  sbc #0
;  sta ?ghostData+1
;
;
;  ldy #13
;  lda (@heap_ptr),y
;  and #1
;  sta ?player_direction
;
;
;  ldy #15                ; Pupils direction
;  lda (@heap_ptr),y
;  sta ?pupils_direction

; --------------------------------------------------------------------------------
; Hier ist der Einsprung vom neuen @draw_ghost
; --------------------------------------------------------------------------------
?inner_copy_ghost
;  lda #0
;  sta colbk

  clc
  lda ?x_pos
  adc #48
  sta ?ghost_x_pos,x
  sta hposp,x            ; x-position des Players

; KOSTEN BIS HIER 8 Zeilen
;  bgcolor $00

; Berechnen, Anzahl Bytes fuer die Augen
  jsr ?calculateBytesForEyes
  sty ?bytesForEyes

; Hintergrund, sichern KOSTEN: 1 Zeile
  lda ?player
  ldy ?bytesForEyes
  jsr ?hintergrund_sichern   ; braucht player number in akku

; Augen in Screen setzen
  jsr ?set_eyes_in_screen    ; KOSTEN: 1 Zeile

;  ldy ?player
;  lda ?pupils_direction
;  cmp ?last_pupils_direction ;,y
;  beq ?pupils_already_set

;  lda ?last_pupils_direction ;,y
;  beq ?no_restore

;?no_restore
  jsr ?insert_pupils         ; KOSTEN: 3 Zeilen

;  ldy ?player
;  lda ?pupils_direction
;  sta ?last_pupils_direction ;,y

;?pupils_already_set

; Augen setzen (Font manipulieren)
  jsr ?insert_eyes           ; KOSTEN: 9 Zeilen

  jsr ?restore_pupils        ; KOSTEN: 2 Zeilen
; Player zeichnen
  jsr ?draw_player           ; KOSTEN: 6 Zeilen

;?unsichtbar
  rts

?calculateBytesForEyes
  lda ?position_in_char
  beq ?twoOrFourBytes

  ldy #3
  rts

?twoOrFourBytes
  lda ?y_pos
  and #7
  beq ?twoBytes

  ldy #4
  rts

?twoBytes
  ldy #2
  rts

?ghost_pointer
 .byte 0,16

; TODO: der alte Player sollte geloescht werden, wenn yposition anders oder direction anders
; zeichnet den Player
?draw_player
  lda #0
  ldy ?player_ypos
  dey                    ; in die oberste Zeile eine 0
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
  lda (?ghostData),y
  sta (?pmadr),y
  iny

  lda (?ghostData),y ; unroll loops 3x
  sta (?pmadr),y
  iny

  lda (?ghostData),y
  sta (?pmadr),y
  iny

  lda (?ghostData),y
  sta (?pmadr),y
  iny

  cpy ?max_copy
  bne ?copy_pm_loop

  lda #0
  sta (?pmadr),y
;  iny
  rts

?player
 .byte 0

?player_direction
 .byte 0

?position_in_char
 .byte 0

?player_ypos
 .byte 0

?bytesForEyes
 .byte 0

?pupils_direction
 .byte 0


; We use this small table to access the ?background structure
?mul_sizeof_background_struct
  .byte 0,8,16,24

; DO NOT CHANGE ANYTHING HERE! --------------
; Das ist eine zusammen gehoerende Struktur
; Player * 8 indiziert ist der Zugriff!
?background
  .byte 0,0,0,0

?background_addr
  .byte 0,0

?background_xpos
  .byte 0

?bytes_for_eyes
  .byte 0

;?background_ypos
;  .byte 0

; 24 weitere bytes fuer den Hintergrund
  .word 0,0,0,0
  .word 0,0,0,0
  .word 0,0,0,0
; BIS HIER!!! -------------------------------

;

; wir stellen den Hintergrund wieder her,
; dazu haben wir uns alles gemerkt
@restoreghost
@restoreghost_i
  ldy #1
  lda (@heap_ptr),y                   ; number of ghost
  and #3
  tax
  lda ?mul_sizeof_background_struct,x ; position in struct
  tax
  stx ?rememberx

  lda ?background_addr,x              ; screen address
  sta ?screenaddr
  lda ?background_addr+1,x
  beq ?nothing_to_restore             ; if we have nothing to restore
  sta ?screenaddr+1

  ldy ?background_xpos,x              ; xposition

  lda ?bytes_for_eyes,x
  cmp #4
  beq ?restore_4_bytes
  cmp #2
  beq ?restore_2_bytes

?restore_3_bytes                      ; restore 3 bytes in xposition  123
  lda ?background,x
  sta (?screenaddr),y
  inx
  iny
?restore_2_bytes                      ; restore 2 bytes               12
  lda ?background,x
  sta (?screenaddr),y
  inx
  iny
  lda ?background,x                   ; restore last byte in xposition
  sta (?screenaddr),y

; it seams we restore sometimes twice
?clear_background_addr
  ldx ?rememberx                      ; we would like to restore only once
  lda #0
  sta ?background_addr+1,x
?nothing_to_restore
  rts

?rememberx
  .byte 0

?restore_4_bytes                      ; restore 4 bytes       12
  lda ?background,x                   ;                       34
  sta (?screenaddr),y
  inx
  iny
  lda ?background,x
  sta (?screenaddr),y
  inx
  tya
  clc
  adc #39
  tay
  lda ?background,x
  sta (?screenaddr),y
  inx
  iny
  lda ?background,x
  sta (?screenaddr),y

  jmp ?clear_background_addr

; ----------------------------------------
; Wir sichern den Hintergrund, in dem wir
; danach die Augen zeichnen
; Das sind nur ein paar Bytes (2, 3 oder 4)

?hintergrund_sichern
;  asl          ; wir uebergeben den aktuellen Player im Akku
;  asl
;  asl          ; * 8 fuer die Struktur
  tax
  lda ?mul_sizeof_background_struct,x
  tax

  tya          ; wir uebergeben bytesForEyes in Y-Reg
  sta ?bytes_for_eyes,x

  lda ?screenaddr
  sta ?background_addr,x
  lda ?screenaddr+1
  sta ?background_addr+1,x

  lda ?byte_position_in_line
  sta ?background_xpos,x
  tay

;  lda ?y_pos
;  sta ?background_ypos,x

  lda ?bytes_for_eyes,x
  cmp #4
  beq ?save_4_bytes
  cmp #2
  beq ?save_2_bytes

; sichere 3 bytes
?save_3_bytes
  lda (?screenaddr),y
  sta ?background,x
  iny
  inx
?save_2_bytes
  lda (?screenaddr),y
  sta ?background,x
  iny
  inx
  lda (?screenaddr),y
  sta ?background,x
  rts

?save_4_bytes
  lda (?screenaddr),y
  sta ?background,x
  iny
  inx
  lda (?screenaddr),y
  sta ?background,x
  tya
  clc
  adc #39
  tay
  inx
  lda (?screenaddr),y
  sta ?background,x
  iny
  inx
  lda (?screenaddr),y
  sta ?background,x

  rts


?set_eyes_in_screen
  lda ?bytesForEyes
  cmp #4
  beq ?set_4_bytes
  cmp #2
  beq ?set_2_bytes

?set_3_bytes
  ldy ?byte_position_in_line
  ldx ?eye_char
  txa
  sta (?screenaddr),y
  inx
  txa
  iny
  sta (?screenaddr),y
  inx
  txa
  iny
  sta (?screenaddr),y
  rts

?set_2_bytes
  ldy ?byte_position_in_line
  ldx ?eye_char
  txa
  sta (?screenaddr),y
  inx
  txa
  iny
  sta (?screenaddr),y
  rts

?set_4_bytes
  ldy ?byte_position_in_line
  ldx ?eye_char
  txa
  sta (?screenaddr),y
  inx
  inx
  txa
  iny
  sta (?screenaddr),y
  tya
  clc
  adc #39
  tay
  dex
  txa
  sta (?screenaddr),y
  inx
  inx
  txa
  iny
  sta (?screenaddr),y

  rts

; Je nach x position innerhalb der ungeraden (nicht durch 4 teilbar)
?xposition_in_eye
  .byte 80, 104, 128, 152

; wir addieren nochmal die Richtung hinzu
?xposition_in_eye_add_direction
 .byte 0,24

?insert_eyes
  lda ?bytesForEyes
  cmp #4
  beq ?change_4_bytes

; 2 or 3 byte position
  ldy ?position_in_char         ; position innerhalb Char (0-3 moeglich)
  lda ?xposition_in_eye,y
  clc
  ldy ?player_direction         ; Richtung des Players (links=0, rechts=1)
  adc ?xposition_in_eye_add_direction,y
  tax
  ldy #0
?insert_loop
; TODO: direkter Zugriff auf die Augen!
  lda gh_eyes,x
  sta (?positionInCharset),y
  inx
  iny

  lda gh_eyes,x               ; unroll loop 3x
  sta (?positionInCharset),y
  inx
  iny

  lda gh_eyes,x
  sta (?positionInCharset),y
  inx
  iny

  lda gh_eyes,x
  sta (?positionInCharset),y
  inx
  iny

  cpy #24
  bne ?insert_loop
  rts

?yposition_in_eye
  .byte 8,7,6,5,4,3,2,1

?yposition_in_eye_add_direction
  .byte 0,40

?change_4_bytes
  lda ?y_pos
  and #7
  tay
  lda ?yposition_in_eye,y
  clc
  ldy ?player_direction
  adc ?yposition_in_eye_add_direction,y
  tax

  ldy #0
?insert_loop2
; TODO: direkter Zugriff auf die Augen!
  lda gh_eyes,x
  sta (?positionInCharset),y
  inx
  iny

  lda gh_eyes,x               ; unroll loop 3x
  sta (?positionInCharset),y
  inx
  iny

  lda gh_eyes,x
  sta (?positionInCharset),y
  inx
  iny

  lda gh_eyes,x
  sta (?positionInCharset),y
  inx
  iny

  cpy #32
  bne ?insert_loop2
  rts

?insert_pupils
  lda ?pupils_direction
  asl a
  asl a
  asl a
  tay

  lda ?bytesForEyes
  cmp #4
  beq ?change_4_bytes_pupils

; 2 or 3 byte position
; diese liegen immer erst ab pupils...3
  clc
  tya
  adc #2
  adc ?position_in_char
  tay
; TODO: add xpos & 3
;  jmp ?copy_pupils

?change_4_bytes_pupils
; die 4 bytes pupils liegen an pupils...1&2
  clc
  tya
  adc ?player_direction
  tay

?copy_pupils
  lda gh_pupils_low,y
  sta ?pupils_loop+1
  sta ?pupils_first+1
  sta ?pupils_second+1

  lda gh_pupils_high,y
  sta ?pupils_loop+2
  sta ?pupils_first+2
  sta ?pupils_second+2
  ldx #0

?pupils_loop
  lda $ffff,x             ; SMC! pupils position
  cmp #255
  beq ?pupils_end

  sta gh_pupils_restore,x
  tay
  inx

  lda gh_eyes,y
  sta gh_pupils_restore,x
?pupils_first
  and $ffff,x             ; SMC! and pupils upper
  sta gh_eyes,y
  inx

  iny
  lda gh_eyes,y
  sta gh_pupils_restore,x
?pupils_second
  and $ffff,x             ; SMC! and pupils lower
  sta gh_eyes,y

  inx
  jmp ?pupils_loop

?pupils_end
  lda #255
  sta gh_pupils_restore,x
  rts


?restore_pupils
  ldx #0

?pupils_loop2
  ldy gh_pupils_restore,x
  cpy #255
  beq ?pupils_end2

  inx

  lda gh_pupils_restore,x
  sta gh_eyes,y
  inx

  iny
  lda gh_pupils_restore,x
  sta gh_eyes,y

  inx
  jmp ?pupils_loop2

?pupils_end2
  rts

?possible_direction=@REG+16
?up=   ~00000001  ; Verschaltung im Uhrzeigersinn
?right=~00000010
?down= ~00000100
?left= ~00001000
?guilty = ~10000000

?no_vertical
?no_horizontal
  ldy #0
  ldx #0
  rts

; ---------------------------------------------------------------
; @check_around
; parameter: x_pos, y_pos in Pixel-Aufloesung (160-8 x 192-16)

; Es wird zuerst geprueft, ob wir an einer durch 4x8 teilbaren
; Position stehen, nur dann werden weitere Tests unternommen,
; sonst setzen wir das Guilty Bit (bit 7) auf 0.
;
; folgende Character-Positionen werden geprueft, wir selbst nehmen
; dann die Position des '#' ein
; .11.
; 4##2
; 4##2
; .33.
; nur wenn beide Werte >= 32 sind, ist die Richtung einschlagbar
; Ist eine Richtung einschlagbar, ist dessen Bit-Wert 1
;
; Aktuell arbeitet diese Funktion nur mit 40 Zeichen pro Zeile
; das ist ziemlich hart kodiert
;
; Ergebnis ist >$80 wenn Gueltig
; Die bits sind wie folgt verteilt
; ~G...LDRU stellt die moeglichen Richtungen dar (1=moeglich) im Uhrzeigersinn
; ~.......U Up
; ~......R. Right
; ~.....D.. Down
; ~....L... Left
;
; TODO: die 3 freien bits sollen fuer die fressbaren buttons verwendet werden.
;
; ---------------------------------------------------------------
@check_around
@check_around_ii
  ldy #1
  lda (@heap_ptr),y
  sta ?x_pos

  ldy #3
  lda (@heap_ptr),y
  sta ?y_pos

; nur wenn y_pos & 7 == 0 and x_pos & 3 == 0 wird der Ring gebaut
?check_infront
  lda ?y_pos
  and #7
  bne ?no_vertical

  lda ?x_pos
  and #3
  bne ?no_horizontal

  lda ?x_pos
  lsr
  lsr
  sta ?byte_position_in_line     ; X-Position innerhalb Screen

  jsr ?calc_screenaddr

  ldx #0 ; ?player

;
; test wall up (Position 11)
;
  sec
  lda ?screenaddr
  sbc #40
  sta ?screenaddr
  lda ?screenaddr+1 ;3
  sbc #0            ;2
  sta ?screenaddr+1 ;3

; TODO umbau?
; bcs nosub         ; 3
; dec ?screenaddr+1 ; 5

  ldy ?byte_position_in_line
  lda (?screenaddr),y
  and #127
  cmp #32
  bcc ?wall_up

  iny
  lda (?screenaddr),y
  and #127
  cmp #32
  bcc ?wall_up

  lda #255
  bne ?not_wall_up

?wall_up
  lda #0
?not_wall_up
  and #?up
  sta ?possible_direction
;  sta @ghost_top_infront,x

; DEBUG: nur um mal anzuzeigen, wo was ausgelesen wird
; lda #1
; ldy ?byte_position_in_line
; sta (?screenaddr),y

;
; test wall left (Position 44)
;
  clc
  lda ?byte_position_in_line
  adc #40-1  ; 39
  tay
  lda (?screenaddr),y

; DEBUG: nur um mal anzuzeigen, wo was ausgelesen wird
;  tax
;  lda #1
;  sta (?screenaddr),y
;  txa

  and #127
  cmp #32
  bcc ?wall_left
  clc
  tya
  adc #40
  tay
  lda (?screenaddr),y
  and #127
  cmp #32
  bcc ?wall_left

  lda #255
  bne ?not_wall_left

?wall_left
  lda #0
?not_wall_left
  and #?left
  ora ?possible_direction
  sta ?possible_direction

;; lda #33
;; sta (?screenaddr),y


;
; test wall right (Position 22)
;
  clc
  lda ?byte_position_in_line
  adc #40+2
  tay
  lda (?screenaddr),y
  and #127
  cmp #32
  bcc ?wall_right
  clc
  tya
  adc #40
  tay
  lda (?screenaddr),y
  and #127
  cmp #32
  bcc ?wall_right

  lda #255
  bne ?not_wall_right

?wall_right
  lda #0
?not_wall_right
  and #?right
  ora ?possible_direction
  sta ?possible_direction

;; lda #65
;; sta (?screenaddr),y

;
; test wall down (Position 33)
;
  clc
  lda ?byte_position_in_line
  adc #40+40+40
  tay
  lda (?screenaddr),y
  and #127
  cmp #32
  bcc ?wall_down

  iny
  lda (?screenaddr),y
  and #127
  cmp #32
  bcc ?wall_down

  lda #255
  bne ?not_wall_down

?wall_down
  lda #0
?not_wall_down
  and #?down
  ora ?possible_direction
  ora #?guilty
;  sta ?possible_direction

; pha
; lda #129
; sta (?screenaddr),y
; pla

  tay
  ldx #0
  rts

; -------------------------------------------------------------
; calculate with y_pos the screenaddress of each character line
?calc_screenaddr
  lda ?y_pos
  lsr
  lsr
  lsr
  tay                    ; ypos / 8 is byte-position in screen

  clc
  lda SAVMSC             ; we add current Screen Address
  adc ?mul40_low,y       ; and 40 time the ypos
  sta ?screenaddr
  lda SAVMSC+1
  adc ?mul40_high,y
  sta ?screenaddr+1
  rts

?mul40_low
  .byte    0, <40,  <80,  <120, <160, <200, <240, <280
  .byte <320, <360, <400, <440, <480, <520, <560, <600
  .byte <640, <680, <720, <760, <800, <840, <880, <920

?mul40_high
  .byte    0, >40,  >80,  >120, >160, >200, >240, >280
  .byte >320, >360, >400, >440, >480, >520, >560, >600
  .byte >640, >680, >720, >760, >800, >840, >880, >920


;; --------------------------------------------------------------
?TWELFTH=@REG+2
?TWENTYFOURTH=@REG+3

@mul13
 .BYTE 0,13,26,39,52,65,78,91

; liefert die Position innerhalb des Playfields
; xp := @getPlayfieldXY(x, y)
@getPlayfieldXY
@getPlayfieldXY_II
 LDY #3
 LDA (@HEAP_PTR),Y
 STA ?y_pos

 cmp #8
 bcc ?returnMinus1

;
; twentyFourth := quotient24[(y - 8)]
;
 SEC
 LDA ?Y_pos
 SBC #<8
 TAY
 LDA ?QUOTIENT24,Y
 STA ?TWENTYFOURTH ; (11)
;
; if twentyFourth == 255 then return 0
;
 cmp #255
 beq ?returnMinus1


 LDY #1
 LDA (@HEAP_PTR),Y
 STA ?x_pos

 cmp #4
 bcc ?returnMinus1

;
; twelfth := quotient12[x - 4]
;
 SEC
 LDA ?X_pos
 SBC #<4
 TAY
 LDA ?QUOTIENT12,Y
 STA ?TWELFTH ; (11)

 cmp #255
 beq ?returnMinus1

;
; xp := twelfth + mul13[ twentyFourth ]
;
 LDY ?TWENTYFOURTH
 LDA @mul13,Y
 CLC
 ADC ?TWELFTH

 TAY
 LDX #0
 RTS

?returnMinus1
 LDY #255
 LDX #255
 rts

; getpositiony(y)
; @return ypf, wenn (y-8)/24 exakt aufgeht, sonst 255 (-1)

@getPlayfieldY
@getPlayfieldY_I
 LDY #1
 LDA (@HEAP_PTR),Y
 STA ?y_pos

 cmp #8
 bcc ?returnMinus1

;
; twentyFourth := quotient24[(y - 8)]
;
 SEC
 LDA ?Y_pos
 SBC #<8
 TAY
 LDA ?QUOTIENT24,Y
 STA ?TWENTYFOURTH ; (11)
;
; if twentyFourth == 255 then return 0
;
 cmp #255
 beq ?returnMinus1

 tay
 ldx #0
 rts

; getpositionx(x)
; @return xpf, wenn (x-4)/12 exakt aufgeht, sonst 255 (-1)
@getPlayfieldX
@getPlayfieldX_I
 LDY #1
 LDA (@HEAP_PTR),Y
 STA ?x_pos

 cmp #4
 bcc ?returnMinus1

;
; twelfth := quotient12[x - 4]
;
 SEC
 LDA ?X_pos
 SBC #<4
 TAY
 LDA ?QUOTIENT12,Y
 STA ?TWELFTH ; (11)

 cmp #255
 beq ?returnMinus1

 tay
 ldx #0
 rts

?QUOTIENT12
 .byte 0,255,255,255,255
 .BYTE 255,255,255,255,255,255,255,1
 .BYTE 255,255,255,255,255,255,255,255
 .BYTE 255,255,255,2,255,255,255,255
 .BYTE 255,255,255,255,255,255,255,3
 .BYTE 255,255,255,255,255,255,255,255
 .BYTE 255,255,255,4,255,255,255,255
 .BYTE 255,255,255,255,255,255,255,5
 .BYTE 255,255,255,255,255,255,255,255
 .BYTE 255,255,255,6,255,255,255,255
 .BYTE 255,255,255,255,255,255,255,7
 .BYTE 255,255,255,255,255,255,255,255
 .BYTE 255,255,255,8,255,255,255,255
 .BYTE 255,255,255,255,255,255,255,9
 .BYTE 255,255,255,255,255,255,255,255
 .BYTE 255,255,255,10,255,255,255,255
 .BYTE 255,255,255,255,255,255,255,11
 .BYTE 255,255,255,255,255,255,255,255
 .BYTE 255,255,255,12,255,255,255,255
 .BYTE 255,255,255,255,255,255,255,13
 .BYTE 255,255,255,255,255,255,255,255

?QUOTIENT24
 .BYTE 0
 .BYTE 255,255,255,255,255,255,255,255
 .BYTE 255,255,255,255,255,255,255,255
 .BYTE 255,255,255,255,255,255,255,1
 .BYTE 255,255,255,255,255,255,255,255
 .BYTE 255,255,255,255,255,255,255,255
 .BYTE 255,255,255,255,255,255,255,2
 .BYTE 255,255,255,255,255,255,255,255
 .BYTE 255,255,255,255,255,255,255,255
 .BYTE 255,255,255,255,255,255,255,3
 .BYTE 255,255,255,255,255,255,255,255
 .BYTE 255,255,255,255,255,255,255,255
 .BYTE 255,255,255,255,255,255,255,4
 .BYTE 255,255,255,255,255,255,255,255
 .BYTE 255,255,255,255,255,255,255,255
 .BYTE 255,255,255,255,255,255,255,5
 .BYTE 255,255,255,255,255,255,255,255
 .BYTE 255,255,255,255,255,255,255,255
 .BYTE 255,255,255,255,255,255,255,6
 .BYTE 255,255,255,255,255,255,255,255
 .BYTE 255,255,255,255,255,255,255,255
 .BYTE 255,255,255,255,255,255,255,7
 .BYTE 255,255,255,255,255,255,255,255
 .BYTE 255,255,255,255,255,255,255,255
 .BYTE 255,255,255,255,255,255,255,8

