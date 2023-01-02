; -*- text -*-
;
; Editor Display List Interrupts
;
; THIS FILE WILL INCLUDE BY OTHER FILE
;
 .local

?ghost_x_pos=133

; Den Displaylist Interrupt lassen wir im VBI setzen, so
; laufen wir nicht Gefahr damit etwas kaputt zu machen

@INITDLI
     LDA #6         ; Nutze Immediate(6) VBI
     LDY #<?RESETDLI
     LDX #>?RESETDLI
     JMP SETVBV


?RESETDLI
     LDA #0
     STA 77 ; no colorswitch

     LDA #$0
     STA NMIEN ; Displaylist Interrupt sperren

;     lda @current_dlist
;     STA SDLSTL
;     LDA @current_dlist+1
;     STA SDLSTL+1
     lda @game_mode
     cmp #1
     beq ?show_title

     jmp ?show_game
;     cmp #1
;     beq ?show_game
;     cmp #3
;     beq ?show_game
;     cmp #4
;     beq ?show_game


?show_title
     LDA #<@PACMEN_TITLE_DLIST
     STA SDLSTL
     LDA #>@PACMEN_TITLE_DLIST
     STA SDLSTL+1

     LDA #<?CHANGE_ZSATZ_TITLE_PACMEN_DLI
     STA VDSLST
     LDA #>?CHANGE_ZSATZ_TITLE_PACMEN_DLI
     STA VDSLST+1

     jmp ?continue_game_mode

?show_game
     LDA #<@PACMEN_GAME_DLIST
     STA SDLSTL
     LDA #>@PACMEN_GAME_DLIST
     STA SDLSTL+1

     LDA #<?CHANGE_ZSATZ_PACMEN_DLI
     STA VDSLST
     LDA #>?CHANGE_ZSATZ_PACMEN_DLI
     STA VDSLST+1

?continue_game_mode

     LDA #$C0
     STA NMIEN ; Displaylist Interrupt ermoeglichen

     lda @colbk_change
     beq ?no_colbk_change

     ldy @colbk_change
     lda ?colbk_colors,y
     sta 712
     dec @colbk_change

?no_colbk_change

     JMP SYSVBV

?colbk_colors
    .byte $00,$00,$10,$10,$11,$12,$13,$14,$15,$16,$17,$18,$1A,$1C,$1E,$1E

;
; TODO: Es besteht die Moeglichkeit noch 320 Zyklen zu sparen, indem wir den DMA fuer
;       die PM-Gfx erst einschalten, wenn wir sie wirklich brauchen
;

?evolution_font_dli
; lda #$0a
; sta colbk

 lda #$B8              ; PACMEN evolution font
; sta wsync
 sta chbase

 lda #$98              ; PACMEN evolution color
 sta COLPF0
 lda #$9a
 sta COLPF1
 lda #$9c
 sta COLPF2

; Versuch ein Umlaut zu setzen
 lda #$1f
 sta colpm0
 lda #107
 sta HPOSP0
; lda #$41
; sta GRAFP0
 sta wsync

 lda ?ghost_x_pos
 nop
 sta wsync
 sta wsync
 sta hposp0
; lda #0
; sta GRAFP0
; lda #0
; sta colbk

 lda 704
 sta colpm0
 rts


?playfield_dli
 lda #$B0              ; PACMEN Game font
; sta wsync
 sta chbase

; lda #3                ; Player Missiles an
; sta GRACTL

; lda #$3e
; sta DMACTL

; lda ?ghost_x_pos
; sta hposp0

; lda 704
; sta colpm0
;
; Farben fuer das Spielfeld
;
 lda @playfield_color   ; #$84
 sta COLPF0
 lda @playfield_color+1 ; #$0F
 sta COLPF1
 lda @playfield_color+2 ; #$36
 sta COLPF2

 lda @playfield_color+3 ; #$1f      ; PACMEN Color (5. Player (4 Missiles))
 sta COLPF3

; lda #$20+$10+$08+$04+$02
; sta DMACTL
;  SDMCTL:=$20 + // DMA
;          $10 + // einzeilige Aufloesung
;          $08 + // DMA Player ein
;          $04 + // DMA Missiles ein
;          $02   // Normales Anzeigefeld (40 bytes)
 rts

; Wir aendern den Zeichensatz auf den Standard zurueck

?CHANGE_ZSATZ_PACMEN_DLI
 PHA
 jsr ?evolution_font_dli
 LDA #<?CHANGE_ZSATZ_GAME_DLI
 STA VDSLST
 LDA #>?CHANGE_ZSATZ_GAME_DLI
 STA VDSLST+1
 PLA
 RTI

?CHANGE_ZSATZ_GAME_DLI
 PHA
 jsr ?playfield_dli

 LDA #<?CHANGE_ZSATZ_HIGHSCORE_LINE_DLI
 STA VDSLST
 LDA #>?CHANGE_ZSATZ_HIGHSCORE_LINE_DLI
 STA VDSLST+1
 PLA
 RTI

?CHANGE_ZSATZ_HIGHSCORE_LINE_DLI
 PHA
 lda #$1c
 sta COLPF0 ; 708
 lda #$9a
 sta COLPF1 ; 709
 lda #$9c
 sta COLPF2 ; 710

 lda #$9e      ; PACMEN Color (5. Player (4 Missiles))
 sta COLPF3

 lda #$AE   ; Pacmen Title Font (Graphics 1)
 sta wsync
 sta chbase

; lda #$20+$02
; sta DMACTL

 PLA
 RTI

; ----------------------------------------------------------
; TITLE DLI
; ----------------------------------------------------------

?CHANGE_ZSATZ_TITLE_PACMEN_DLI
 PHA
 jsr ?evolution_font_dli

 LDA #<?CHANGE_ZSATZ_TITLE_DLI
 STA VDSLST
 LDA #>?CHANGE_ZSATZ_TITLE_DLI
 STA VDSLST+1
 PLA
 RTI

?CHANGE_ZSATZ_TITLE_DLI
 PHA
 lda #$AE              ; Normal Font
; sta wsync
 sta chbase

;
; Farben fuer graphics 1 Title
;
 lda #$84
 sta COLPF0
 lda #$0F
 sta COLPF1
 lda #$36
 sta COLPF2

 lda #$1f      ; PACMEN Color (5. Player (4 Missiles))
 sta COLPF3

 LDA #<?CHANGE_ZSATZ_GAME_SUBSET_DLI
 STA VDSLST
 LDA #>?CHANGE_ZSATZ_GAME_SUBSET_DLI
 STA VDSLST+1
 PLA
 RTI

?CHANGE_ZSATZ_GAME_SUBSET_DLI
 PHA
 jsr ?playfield_dli

 LDA #<?CHANGE_ZSATZ_TITLE_OTHER_DLI
 STA VDSLST
 LDA #>?CHANGE_ZSATZ_TITLE_OTHER_DLI
 STA VDSLST+1
 PLA
 RTI


?CHANGE_ZSATZ_TITLE_OTHER_DLI
 PHA
 lda #$AE      ; Normal Font
 sta wsync
 sta chbase

 lda #$84
 sta COLPF0
 lda #$0F
 sta COLPF1
 lda #$36
 sta COLPF2

 lda #$1f      ; PACMEN Color (5. Player (4 Missiles))
 sta COLPF3

 PLA
 RTI
