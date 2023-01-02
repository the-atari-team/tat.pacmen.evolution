;;  Pacmen Audio fuer Game, no music, only some audio

; .include "NOTEN.INC"
  .local

@pacmen_audio
    .word ?audio_Length        ; Zeigt auf 8 Byte Liste [0] ist volle Note,[1] halbe...
    .word ?audio_huellkurven
    .word ?audio_Shapes
    .word 0                    ; 3. Parameter von music_init, wo faengt die Liste der Takte an

; This must be 8 bytes after the table before :-(
?audio_Length
       .byte 0,0,0,0,0,0,0,0

?audio_huellkurven
  .word 0
  .word ?huell_EAT_COOKIE     ; 1
  .word 0 ; ?huell_EAT_COOKIE2
  .word ?huell_AUGEN          ; 3
  .word ?huell_AUGEN_ENDE     ; 4
  .word ?huell_GHOST_KILL     ; 5
  .word ?huell_EAT_FAT_COOKIE ; 6
  .word ?huell_EAT_COLLECTABLE ; 7
  .word ?huell_PACMEN_STIRBT  ; 8
  .word ?huell_B3_DOWN        ; 9
  .word 0 ; ?huell_EXTRA_LEVEL
  .word ?huell_BUGLE_MUTED_CHARGE           ; 11
  .word ?huell_BUGLE_MUTED_CHARGE_OVERTONE  ; 12
  .word ?huell_TIC   ; 13
  .word ?huell_TOC   ; 14
  .word ?huell_EMPTY ; 15

?audio_Shapes
  .word 0
  .word ?shape_EAT_COOKIE
  .word 0 ; ?shape_EAT_COOKIE2
  .word ?shape_AUGEN
  .word ?shape_AUGEN_ENDE
  .word ?shape_GHOST_KILL
  .word ?shape_EAT_FAT_COOKIE
  .word ?shape_EAT_COLLECTABLE
  .word ?shape_PACMEN_STIRBT
  .word ?shape_B3_DOWN
  .word 0 ; ?shape_EXTRA_LEVEL
  .word ?shape_BUGLE_MUTED_CHARGE           ; 11
  .word ?shape_BUGLE_MUTED_CHARGE_OVERTONE  ; 12
  .word ?shape_BUGLE_MUTED_CHARGE_OVERTONE_2
  .word 0
  .word ?shape_EXTRA_FANFARE                ; 15
  .word ?shape_EXTRA_FANFARE_OVERTONE
  .word ?shape_TIC_TOC                      ; 17
  .word ?shape_TIC_TOC_stop                 ; 18


?huell_EAT_COOKIE ; 1
  .byte $B7,$04,$B7,$04,$B7,$04,$B6,$04,$B5,$04,$B6,$04,$B5,$04,$B5,$04,$B4,$04,$B2,$04,$B1,$04,$B0,$00
  .byte ENDE

?shape_EAT_COOKIE
  .BYTE HUELL,1
  .byte CIS4,H,ENDE



;?huell_EAT_COOKIE2
;  .byte $11,$02,$13,$02,$17,$02,$16,$02,$15,$02,$16,$02,$15,$02,$15,$02,$14,$02,$12,$02,$11,$02,$10,$00
;  .byte ENDE
;
;?shape_EAT_COOKIE2
;  .BYTE HUELL,2
;  .byte FIS4,H,ENDE
;


; Braucht Speed := 3!
?huell_AUGEN ; 3
  .byte $B4,$01,$b4,$02,$b4,$03,$b4,$04,$b4,$05,$b4,$06
  .byte $B4,$Fa,$b4,$Fb,$b4,$Fc,$b4,$Fd,$b4,$Fe,$b4,$FF,$00,ENDE

?shape_AUGEN
  .BYTE HUELL,3
  .byte C4,E,CIS4,E,D4,E,DIS4,E,DIS4,E,D4,E,CIS4,E,C4,E,START



?huell_AUGEN_ENDE ; 4
  .byte $a4,$a4,$a4,$a4,$a3,$a3,$a3,$a3
  .byte $a2,$a2,$a2,$a2,$a1,$a1,$a1,$a1,$a0,ENDE

?shape_AUGEN_ENDE
  .BYTE HUELL,4
  .byte C4,E,ENDE



?huell_GHOST_KILL ; 5
  .byte $04,$08,$0b,$0a,$09,$08,$07
  .byte $89,$88,$87,$85,$85,$84,$84,$83,$83,$83,$82,$82,$82,$81,$81,$81,$80,ENDE

?shape_GHOST_KILL
  .byte HUELL,5
  .byte FIS1,E,ENDE



?huell_EAT_FAT_COOKIE ; 6
  .byte $92,$01,$94,$01,$96,$01,$98,$02,$97,$02,$98,$02,$97,$03,$98,$03
  .byte $17,$03,$16,$04,$15,$04,$14,$04,$14,$05,$13,$05,$13,$05,$12,$06,$12,$06,$11,$06,$11,$08,$00,ENDE

?shape_EAT_FAT_COOKIE
  .byte HUELL,6
  .byte AIS3,E,ENDE



?huell_EAT_COLLECTABLE ; 7
  .byte $A8,$A6,$A4,$B2,$f0
  .byte $A7,$A5,$A3,$B1,$f0
  .byte $A8,$A6,$A4,$B2,$f0
  .byte $A7,$A5,$A3,$A1,$00,ENDE

?shape_EAT_COLLECTABLE
  .byte HUELL,7
  .byte FIS5,E,ENDE



?huell_PACMEN_STIRBT ; 8
  .byte $B3,$F8,$A3,$B3,$F8,$A3
  .byte $B4,$F8,$A4,$B4,$F8,$A4
  .byte $B5,$F8,$A5,$B5,$F8,$A5
  .byte $B6,$F8,$A6,$B6,$F8,$A6
  .byte $B7,$F8,$A7,$B7,$F8,$A7
  .byte $B8,$F8,$A8,$B8,$F8,$A8 ; 24f
  .byte $00,ENDE

?shape_PACMEN_STIRBT
  .byte HUELL,8
  .byte A3,Q
  .byte pause,s
  .byte HUELL,5
  .byte G0,Q,ENDE



?huell_B3_DOWN ; 9
  .byte $B7,$FF,$B7,$FF,$B7,$FF,$B7,$FF,$B7,$FF,$B7,$FF
  .byte $B7,$FF,$B7,$FF,$B7,$FF,$B7,$FF,$B7,$FF,$B7,$FF
  .byte $A6,$A6
  .byte $A5,$A5
  .byte $A4,$A4
  .byte $A3,$A3
  .byte $A2,$A2
  .byte $A1,$A1
  .byte $00,ENDE

?shape_B3_DOWN
  .byte HUELL,9
  .byte B3,E,A3,E,G3,E,F3,E,E3,E,D3,E,C3,E,ENDE

;?huell_EXTRA_LEVEL
;.byte $B7,$10,$B7,$10,$B7,$10,$B7,$10,$B7,$10,$B7,$10,$B7,$10,$B7,$10,$B7,$10,$B7,$10,$B7,$10,$B7,$10
;.byte $B7,$20,$B7,$20,$B7,$20,$B7,$20,$B7,$20,$B7,$20,$B7,$20,$B7,$20,$B7,$20,$B7,$20,$B7,$20,$B7,$20
;.byte $B7,$40,$B7,$40,$B7,$40,$B7,$40,$B7,$40,$B7,$40,$B7,$40,$B7,$40,$B7,$40,$B7,$40,$B7,$40,$B7,$40
;.byte $B7,$80,$B7,$80,$B7,$80,$B7,$80,$B7,$80,$B7,$80,$B7,$80,$B7,$80,$B7,$80,$B7,$80,$B7,$80,$B7,$80,ENDE
;
;?shape_EXTRA_LEVEL
;  .byte HUELL,10
;  .byte B3,F,ENDE


?huell_BUGLE_MUTED_CHARGE ; 11
  .byte $B1,$00,$A2,$A3,$A5,$Aa,$Aa,$A9,$A9,$Aa,$Aa,$A9,$A9
  .byte $Aa,$Aa,$A9,$A9,$Aa,$Aa,$A9,$A8,$A8,$A7,$A6,$A6
  .byte $A5,$A4,$A3,$A3,$A2,$A1,$A1,$A0,ENDE

?huell_BUGLE_MUTED_CHARGE_OVERTONE ; 12
  .byte $B1,$00,$A2,$A3,$A5,$A9,$A9,$A8,$A8,$A9,$A9,$A8,$A8
  .byte $A9,$A9,$A8,$A8,$A9,$A9,$A8,$A7,$A7,$A6,$A5,$A5
  .byte $A4,$A3,$A2,$A2,$A1,$A1,$A1,$A0,ENDE

?shape_BUGLE_MUTED_CHARGE
  .byte HUELL,11
  .byte G3,E,C4,E,E4,E,G4,E,P,S,E4,E,G4,E,ENDE

?shape_BUGLE_MUTED_CHARGE_OVERTONE
  .byte HUELL,12
;  .byte G3,E,C4,E,E4,E,G4,E,P,S,E4,E,G4,E,ENDE
;  .byte G3+2,E,C4+2,E,E4+2,E,G4+2,E,P,S,E4+2,E,G4+2,E,ENDE
  .byte G4,E,C5,E,E5,E,G5,E,P,S,E5,E,G5,E,ENDE
;  .byte G5,E,C6,E,E6,E,G6,E,P,S,E6,E,G6,E,ENDE

?shape_BUGLE_MUTED_CHARGE_OVERTONE_2
  .byte HUELL,12
  .byte G5,E,C6,E,E6,E,G6,E,P,S,E6,E,G6,E,ENDE

?shape_EXTRA_FANFARE
  .byte HUELL,11
  .byte E4,E,P,S,G4,E,ENDE

?shape_EXTRA_FANFARE_OVERTONE
  .byte HUELL,12
  .byte E5,E,P,S,G5,E,ENDE

?huell_TIC
 .byte $23,$22,$20,ENDE

?huell_TOC
 .byte $23,$22,$21,$20,ENDE

?shape_TIC_TOC
 .byte HUELL,13,CIS0,Q
 .byte HUELL,14,D0,Q
 .byte HUELL,13,CIS0,Q
 .byte HUELL,14,D0,Q
 .byte START

?huell_EMPTY
 .byte $a0,$a0,ENDE

?shape_TIC_TOC_stop
 .byte HUELL,15,D0,Q,ENDE

;; sinus Amp=10 len 12
;  .byte $A5,$a7,$a9,$aa,$a9,$a8,$a5,$a3,$a1,$a0,$a1,$a2
;
;; sinus Amp=8 len 12
;  .byte $A4,$a6,$a7,$a8,$a7,$a6,$a4,$a2,$a1,$a0,$a1,$a2
;
;; sinus Amp=6 len 12
;  .byte $A3,$a4,$a6,$a6,$a6,$a5,$a3,$a2,$a0,$a0,$a0,$a1
;
;; sinus Amp=4 len 12
;  .byte $A2,$a3,$a4,$a4,$a4,$a3,$a2,$a1,$a0,$a0,$a0,$a1
;
;; sinus Amp=2 len 12
;  .byte $A1,$a1,$a2,$a2,$a2,$a2,$a1,$a1,$a0,$a0,$a0,$a0
;
;; sinus Amp=10 len 12 0-270 Grad abfallend
;  .byte 5,6,7,8,7,6,5,4,2,1,0,0
;
;  .byte 7,8,10,10,10,8,7,5,3,2,1,0
;
;; sinus amp=10 len 24 0-270 Grad abfallend
;  .byte 7,8,9,10,10,10,10,10,9,9,8,7,  6,5,4,3,2,2,1,1,0,0,0,0



;?audio_takt
;    .byte AUDIO_INIT, SPEED_3, AUDCTL_0, 0 ; Hi-Speed
;    .byte 1,0,0,0
;    .byte ENDE,0,0,0

@audio_takt_muted_charge
    .byte AUDIO_INIT, SPEED_1, AUDCTL_0, 0 ; Hi-Speed
    .byte 11,12,0,0
    .byte ENDE,0,0,0

@audio_takt_extra_fanfare
    .byte AUDIO_INIT, SPEED_2, AUDCTL_0, 0 ; Hi-Speed
    .byte 15,16,0,0
    .byte ENDE,0,0,0
