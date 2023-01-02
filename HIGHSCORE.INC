; Current score and high score for Pacmen

      .local

; Wir arbeiten hier mit BCD Zahlen, dann haben wir es bei der Ausgabe einfacher
; einfach als Hex-Zahlen (3x1 Byte) rueckwaerts ausgeben
; Das heisst aber:
; in Interrupts ist unbedingt CLD zu verwenden, sollte ADC/SBC zum Einsatz kommen
;
; Da @printff('%x%x%x%x', a,b,c,d) zu teuer ist (sehr viele Bildschirmzeilen),
; implementieren wir das Setzen der (High-)Scores hier einfach nochmal.
; Das setzen geht damit recht flott, Y:=@current_cursor_row) X:=colcrs
;
;
; WICHTIG: Da wir hier BCD nutzen, den Wert hier als hex uebergeben,
; aber nur die Zahlen 0-9 sind zulaessig. Max $9999 ist moeglich!
; erhoehe Highscore um Wert aus Parameter
@ADD_CURRENT_SCORE
@ADD_CURRENT_SCORE_I
 LDY #1
 LDA (@HEAP_PTR),Y ; wert holen
 sta @reg
 iny
 lda (@heap_ptr),y
 sta @reg+1

 lda @pacmen_multiply_score_10
 beq ?no_mult

 asl @reg      ; um 4 bit verschieben, entspricht in BCD * 10
 rol @reg+1
 asl @reg
 rol @reg+1
 asl @reg
 rol @reg+1
 asl @reg
 rol @reg+1

?no_mult
 SED
 CLC
 lda @reg
 ADC @BCD_CURRENT_SCORE
 STA @BCD_CURRENT_SCORE

 lda @reg+1
 ADC @BCD_CURRENT_SCORE+1
 STA @BCD_CURRENT_SCORE+1

 LDA @BCD_CURRENT_SCORE+2
 ADC #$00
 STA @BCD_CURRENT_SCORE+2
 LDA @BCD_CURRENT_SCORE+3
 ADC #$00
 STA @BCD_CURRENT_SCORE+3

 CLD
 RTS

; ueberprueft current score mit Highscore und kopiert wenn current > high
@CHECK_HIGHSCORE
 LDA @BCD_CURRENT_SCORE+3
 CMP @BCD_HIGHSCORE+3
 BMI ?DO_NOT_COPY_CURRENT_TO_HIGH
 BEQ ?CHECK_NO_3

 LDA @BCD_CURRENT_SCORE+3
 STA @BCD_HIGHSCORE+3
?COPY_3
 LDA @BCD_CURRENT_SCORE+2
 STA @BCD_HIGHSCORE+2
?COPY_2
 LDA @BCD_CURRENT_SCORE+1
 STA @BCD_HIGHSCORE+1
?COPY_1
 LDA @BCD_CURRENT_SCORE
 STA @BCD_HIGHSCORE

 ldy #1                  ; es gibt einen neues Highscore
 sty @new_highscore
 ldx #0
 RTS

?CHECK_NO_3
 LDA @BCD_CURRENT_SCORE+2
 CMP @BCD_HIGHSCORE+2
 BMI ?DO_NOT_COPY_CURRENT_TO_HIGH
 BEQ ?CHECK_NO_2
 JMP ?COPY_3

?CHECK_NO_2
 LDA @BCD_CURRENT_SCORE+1
 CMP @BCD_HIGHSCORE+1
 BMI ?DO_NOT_COPY_CURRENT_TO_HIGH
 BEQ ?CHECK_NO_1
 JMP ?COPY_2

?CHECK_NO_1
 LDA @BCD_CURRENT_SCORE
 CMP @BCD_HIGHSCORE
 BMI ?DO_NOT_COPY_CURRENT_TO_HIGH
 JMP ?COPY_1

?DO_NOT_COPY_CURRENT_TO_HIGH
 ldy #0
 ldx #0
 RTS

; setzt den aktuellen Score auf 0 zurueck
@init_current_score
 LDA #0
 STA @BCD_CURRENT_SCORE
 STA @BCD_CURRENT_SCORE+1
 STA @BCD_CURRENT_SCORE+2
 STA @BCD_CURRENT_SCORE+3

 sta @new_highscore      ; ist nur 1, wenn es einen neuen Highscore gibt
 RTS

@remember_current_score
  lda @bcd_current_score
  sta @bcd_remember_score
  lda @bcd_current_score+1
  sta @bcd_remember_score+1
  rts

; @return 1, if first two bytes of current_score has changed to remember_score
@has_current_score_changed
  lda @bcd_current_score
  cmp @bcd_remember_score
  bne ?score_has_changed    ; --> bytes unterscheiden sich
  lda @bcd_current_score+1
  cmp @bcd_remember_score+1
  bne ?score_has_changed    ; --> obere bytes unterscheiden sich

  ldy #0
  ldx #0
  rts

?score_has_changed
  ldy #1
  ldx #0
  rts

@bcd_remember_score
 .byte 0,0

@BCD_CURRENT_SCORE       ; big endian!
 .byte 0,0,0,0

@new_highscore
 .byte 0

@BCD_HIGHSCORE
 .byte 0,0,0,0

; Kopiert den aktuellen Highscore an die gewuenschte Cursor-Position
; Die Cursor-Position kann mit @fast_gotoxy(x,y) gesetzt werden
; @parameter void
@print_highscore_in_title
 ldy #20
@print_highscore
 jsr ?load_current_cursor_row_and_set_Y_from_colcrs

 lda @BCD_HIGHSCORE+3
 jsr ?print_bcd
 lda @BCD_HIGHSCORE+2
 jsr ?print_bcd
 lda @BCD_HIGHSCORE+1
 jsr ?print_bcd
 lda @BCD_HIGHSCORE
 jmp ?print_bcd

; Initialisiert die internen Register zum setzen der Zahlen an die richtige Position
?load_current_cursor_row_and_set_Y_from_colcrs
 lda @current_cursor_row
 sta @reg
 lda @current_cursor_row+1
 sta @reg+1

 ldy COLCRS
 rts

; Kopiert den aktuellen Score an die gewuenschte Cursor-Position
; Die Cursor-Position kann mit @fast_gotoxy(x,y) gesetzt werden
; @parameter void
@print_score_in_title
 ldy #20
@print_score
 jsr ?load_current_cursor_row_and_set_Y_from_colcrs

 lda @bcd_current_score+3
 jsr ?print_bcd
 lda @bcd_current_score+2
 jsr ?print_bcd
 lda @bcd_current_score+1
 jsr ?print_bcd
 lda @bcd_current_score
 jmp ?print_bcd

; Interne Funktion, um den Akku als 2 BCD-Zahlen in den Screen zu setzen
; (@reg),y zeigt in den Screen
; goody: @score_color wird zur Zahl hinzuge"oder"t damit koennen unterschiedliche
; Farben gesetzt werden.
?print_bcd
 pha
 lsr
 lsr
 lsr
 lsr
 tax
 lda @screen_digit,x
 ora @score_color,y
 sta (@reg),y
 iny
 pla
 and #$0F
 tax
 lda @screen_digit,x
 ora @score_color,y
 sta (@reg),y
 iny
 rts

@screen_digit
 .sbyte '0,'1,'2,'3,'4,'5,'6,'7,'8,'9

@score_color
 .byte $c0,$c0,$c0,$c0,$c0,$c0,$c0,$c0,$c0,$c0
 .byte $c0,$c0,$c0,$c0,$c0,$c0,$c0,$c0,$c0,$c0
 .byte $c0,$c0,$c0,$c0,$c0,$c0,$c0,$c0,0,0
 .byte 0,0,$40,$40,$40,$40,$40,$40,$40,$40
