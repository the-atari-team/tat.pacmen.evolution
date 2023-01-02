;;  Pacmen Music fuer Game

; .include "NOTEN.INC"

  .local

;    *=$aa00
@pacmen_music
    .word ?Length           ; Zeigt auf 8 Byte Liste [0] ist volle Note,[1] halbe...
    .word ?huellkurven_array
    .word ?Shapes_array
    .word ?takt_array       ; wo faengt die Liste der Takte an

; ********************************************************************************
?Length
       .byte 0,0,0,0,0,0,0,0

; Liste der Huellkurven
; up to 127 huellkurven

?huellkurven_array
       .WORD 0
       .word ?HUELL_BASS, ?HUELL_DRUMS, ?HUELL_PACMEN, ?HUELL4, ?HUELL5
       .word ?HUELL6


?HUELL_BASS

;       .byte  3,7,7,6,6,6,6,6,6,5,5,4,3,2,1,1,1,1,1,0,ENDE,0,0,0,0,0,0,0,0,0,0,0
       .BYTE $AA,$A9,$A8,$A7,$A7,$A7,$A7,$A7,$A7,$A6,$A6,$A5,$A4,$A3,$A2,$A2,$A1,$A1,$A1,$A0,ENDE


?HUELL_DRUMS
       .BYTE $85,$84,$82,$81,$80,ENDE


?HUELL_PACMEN
;       .BYTE $AA,$AB,$A9,$A8,$A7,$A7,$A7,$A7,$A7,$A6,$A6,$A5,$A4,$A3,$A2,$A2,$A1,$A1,$A1,$A0,$A0,$A0,$A0,$A0,$A0,$A0,$A0,$A0,$A0,$A0,$A0,$A0,ENDE
        .BYTE $AA,$A9,$A9,$A8,$A8,$A7,$A7,$A6,$A6,$A5,$A5,$A4,$A4,$A3,$A3,$A2,$A2,$A1,$A1,$A0,ENDE ; 2. Teile Saege


?HUELL4
       .byte $A1,$A1,$A2,$A3,$A5,$A7,$A9,$A9,$A0,ENDE


?HUELL5
       .BYTE $A1,$A3,$A5,$A9,$A0,ENDE


?HUELL6
       .BYTE $AA,$A9,$A9,$A8,$A8,$A7,$A7,$A6,$A6,$A5,$A5,$A4,$A4,$A3,$A3,$A2,$A2,$A1,$A1,$A1,$A1,$A1,$A1,$A1,$A1,$A1,$A1,$A1,$A1,$A1,$A1,$A1,$A1,$A1,$A1,$A1,$A1,$A1,$A0,ENDE


; ***************************************************************************---
; Adressenliste der Shapes
; up to 127 Shapes (0-126)
?Shapes_array
    .word 0, ?shape_PACMEN_O5_1, ?shape_PACMEN_O5_2, ?shape_PACMEN_O5_up, ?shape_PACMEN_O5_down
    .word 0, ?shape_PACMEN_O4_1, ?shape_PACMEN_O4_2, ?shape_PACMEN_O4_up, ?shape_PACMEN_O4_down
    .word 0, ?shape_PACMEN_O3_1, ?shape_PACMEN_O3_2, ?shape_PACMEN_O3_up, 0
    .word 0, ?shape_PACMEN_O5_1r,?shape_PACMEN_O5_2r,?shape_PACMEN_O5_upr,0
    .word 0,0,0,0,0
    ; 25
    .word ?shape_DRUMS,?shape_DRUMS_2,?shape_DRUMS_3,0,0
    ; 30
    .word ?shape_BASS3_UP,?shape_BASS3_DOWN
    ; 32
    .word ?shape_BASS_GIS0,?shape_BASS_A0,?shape_BASS_AIS0
    ; 35 ($23)
    .word ?shape_BASS_B0,?shape_BASS_C1,0,0,0
    ; 40
    .word ?shape_BASS_B0_auslaufend

  .word ?shape_intermediate_part_1, ?shape_intermediate_part_2, ?shape_intermediate_part_3
  .word ?shape_intermediate_part_4, ?shape_intermediate_part_5, ?shape_intermediate_part_6
  .word ?shape_intermediate_part_7, ?shape_intermediate_part_8,0

  .word 0
  .word ?shape_intermediate_part_11, ?shape_intermediate_part_12, ?shape_intermediate_part_13
  .word ?shape_intermediate_part_14, ?shape_intermediate_part_15, ?shape_intermediate_part_16
  .word ?shape_intermediate_part_17, ?shape_intermediate_part_18,0

  .word 0
  .word ?shape_intermediate_part_21, ?shape_intermediate_part_22, ?shape_intermediate_part_23
  .word ?shape_intermediate_part_24, ?shape_intermediate_part_25, ?shape_intermediate_part_26
  .word ?shape_intermediate_part_27, ?shape_intermediate_part_28,0

?shape_intermediate_part_1
  .byte HUELL,3
  .byte c3,e,c4,e,g3,e,e3,e,      c4,e,e3,e,g3,e,c3,e,ENDE

?shape_intermediate_part_2
  .byte HUELL,3
  .byte dis3,e,c3,e,dis3,e,f3,e,  g3,e,c3,e,dis3,e,c3,e,ENDE

?shape_intermediate_part_3
  .byte HUELL,3
  .byte c3,e,c4,e,g3,e,e3,e,      c4,e,g3,e,e3,e,c3,e,ENDE

?shape_intermediate_part_4
  .byte HUELL,3
  .byte f3,q,f3,q,                dis3,q,c3,q,ENDE

?shape_intermediate_part_5
  .byte HUELL,3
  .byte c3,e,c4,e,g3,e,e3,e,      c4,e,e3,e,c4,e,e3,e,ENDE

?shape_intermediate_part_6
  .byte HUELL,3
  .byte g3,e,c3,e,dis3,e,c3,e,    dis3,e,f3,e,g3,e,c3,e,ENDE

?shape_intermediate_part_7
  .byte HUELL,3
  .byte dis3,e,c3,e,c3,h,    dis3,q,ENDE

?shape_intermediate_part_8
  .byte HUELL,3
  .byte b2,q,c3,h,c3,h,ENDE



?shape_intermediate_part_11
  .byte HUELL,3
  .byte c4,e,c5,e,g4,e,e4,e,      c5,e,e4,e,g4,e,c4,e,ENDE

?shape_intermediate_part_12
  .byte HUELL,3
  .byte dis4,e,c4,e,dis4,e,f4,e,  g4,e,c4,e,dis4,e,c4,e,ENDE

?shape_intermediate_part_13
  .byte HUELL,3
  .byte c4,e,c5,e,g4,e,e4,e,      c5,e,g4,e,e4,e,c4,e,ENDE

?shape_intermediate_part_14
  .byte HUELL,3
  .byte f4,q,f4,q,                dis4,q,c4,q,ENDE

?shape_intermediate_part_15
  .byte HUELL,3
  .byte c4,e,c5,e,g4,e,e4,e,      c5,e,e4,e,c5,e,e4,e,ENDE

?shape_intermediate_part_16
  .byte HUELL,3
  .byte g4,e,c4,e,dis4,e,c4,e,    dis4,e,f4,e,g4,e,c4,e,ENDE

?shape_intermediate_part_17
  .byte HUELL,3
  .byte dis4,e,c4,e,c4,h,    dis4,q,ENDE

?shape_intermediate_part_18
  .byte HUELL,3
  .byte b3,q,c4,h,c4,h,ENDE



?shape_intermediate_part_21
  .byte HUELL,3
  .byte c5,e,c6,e,g5,e,e5,e,      c6,e,e5,e,g5,e,c5,e,ENDE

?shape_intermediate_part_22
  .byte HUELL,3
  .byte dis5,e,c5,e,dis5,e,f5,e,  g5,e,c5,e,dis5,e,c5,e,ENDE

?shape_intermediate_part_23
  .byte HUELL,3
  .byte c5,e,c6,e,g5,e,e5,e,      c6,e,g5,e,e5,e,c5,e,ENDE

?shape_intermediate_part_24
  .byte HUELL,3
  .byte f5,q,f5,q,                dis5,q,c5,q,ENDE

?shape_intermediate_part_25
  .byte HUELL,3
  .byte c5,e,c6,e,g5,e,e5,e,      c6,e,e5,e,c6,e,e5,e,ENDE

?shape_intermediate_part_26
  .byte HUELL,3
  .byte g5,e,c5,e,dis5,e,c5,e,    dis5,e,f5,e,g5,e,c5,e,ENDE

?shape_intermediate_part_27
  .byte HUELL,3
  .byte dis5,e,c5,e,c5,h,    dis5,q,ENDE

?shape_intermediate_part_28
  .byte HUELL,3
  .byte b4,q,c5,h,c5,h,ENDE

?shape_PACMEN_O5_1
 .BYTE HUELL,3
 .byte B4,  E,     B5,E,FIS5,E,     DIS5,E,B5,S,FIS5,E,       DIS5,E,PAUSE,S,PAUSE,E,ENDE
;.byte B3,  E,     B4,E,FIS4,E,     DIS4,E,B4,S,FIS4,E,       DIS4,E,PAUSE,S,PAUSE,E,ENDE


?shape_PACMEN_O5_2
 .BYTE HUELL,3
 .byte C5,  E,     C6,E,G5,  E,     E5,  E,C6,S,G5,  E,       E5,  E,PAUSE,S,PAUSE,E,ENDE
;.byte C4,  E,     C5,E,G4,  E,     E4,  E,C5,S,G4,  E,       E4,  E,PAUSE,S,PAUSE,E,ENDE

?shape_PACMEN_O5_UP
 .BYTE HUELL,3
 .byte DIS5,S,E5,S,F5,E,F5,S,FIS5,S,G5,  E,G5,S,GIS5,S,A5,  E,A5,S,AIS5,S, B5,E,ENDE
;.byte DIS4,S,E4,S,F4,E,F4,S,FIS4,S,G4,  E,G4,S,GIS4,S,A4,  E,A4,S,AIS4,S, B4,E,ENDE

?shape_PACMEN_O5_DOWN
 .BYTE HUELL,3
 .byte DIS5,S,D5,S,C5,E,C5,S,B4,S,  AIS4,E,AIS4,S,A4,S,GIS4,E,GIS4,S,G4,S, F4,E,ENDE
;.byte DIS4,S,D4,S,C4,E,C4,S,B3,S,  AIS3,E,AIS3,S,A3,S,GIS3,E,GIS3,S,G3,S, F3,E,ENDE

?shape_PACMEN_O4_1
 .BYTE HUELL,3
 .byte B3,  E,     B4,E,FIS4,E,     DIS4,E,B4,S,FIS4,E,       DIS4,E,PAUSE,S,PAUSE,E,ENDE
;.byte B2,  E,     B3,E,FIS3,E,     DIS3,E,B3,S,FIS3,E,       DIS3,E,PAUSE,S,PAUSE,E,ENDE

?shape_PACMEN_O4_2
 .BYTE HUELL,3
 .byte C4,  E,     C5,E,G4,  E,     E4,  E,C5,S,G4,  E,       E4,  E,PAUSE,S,PAUSE,E,ENDE
; .byte C3,  E,     C4,E,G3,  E,     E3,  E,C4,S,G3,  E,       E3,  E,PAUSE,S,PAUSE,E,ENDE

?shape_PACMEN_O4_UP
 .BYTE HUELL,3
 .byte DIS4,S,E4,S,F4,E,F4,S,FIS4,S,G4,  E,G4,S,GIS4,S,A4,  E,A4,S,AIS4,S, B4,E,ENDE
;.byte DIS3,S,E3,S,F3,E,F3,S,FIS3,S,G3,  E,G3,S,GIS3,S,A3,  E,A3,S,AIS3,S, B3,E,ENDE

?shape_PACMEN_O4_DOWN
 .BYTE HUELL,3
 .byte DIS4,S,D4,S,C4,E,C4,S,B3,S,  AIS3,E,AIS3,S,A3,S,GIS3,E,GIS3,S,G3,S, F3,E,ENDE
;.byte DIS3,S,D3,S,C3,E,C3,S,B2,S,  AIS2,E,AIS2,S,A2,S,GIS2,E,GIS2,S,G2,S, F2,E,ENDE


?shape_PACMEN_O3_1
 .BYTE HUELL,3
 .byte B2,  E,     B3,E,FIS3,E,     DIS3,E,B3,S,FIS3,E,       DIS3,E,PAUSE,S,PAUSE,E,ENDE
;.byte B1,  E,     B2,E,FIS2,E,     DIS2,E,B2,S,FIS2,E,       DIS2,E,PAUSE,S,PAUSE,E,ENDE

?shape_PACMEN_O3_2
 .BYTE HUELL,3
 .byte C3,  E,     C4,E,G3,  E,     E3,  E,C4,S,G3,  E,       E3,  E,PAUSE,S,PAUSE,E,ENDE
;.byte C2,  E,     C3,E,G2,  E,     E2,  E,C3,S,G2,  E,       E2,  E,PAUSE,S,PAUSE,E,ENDE

?shape_PACMEN_O3_UP
 .BYTE HUELL,3
 .byte DIS3,S,E3,S,F3,E,F3,S,FIS3,S,G3,  E,G3,S,GIS3,S,A3,  E,A3,S,AIS3,S, B3,E,ENDE
;.byte DIS2,S,E2,S,F2,E,F2,S,FIS2,S,G2,  E,G2,S,GIS2,S,A2,  E,A2,S,AIS2,S, B2,E,ENDE


?shape_PACMEN_O5_1r
 .BYTE HUELL,4
 .byte B4,E, B5,E, FIS5,E, DIS5,E
 .byte HUELL,5, B5,S
 .byte HUELL,4, FIS5,E, DIS5,E,PAUSE,S,PAUSE,E,ENDE

?shape_PACMEN_O5_2r
 .BYTE HUELL,4
 .byte C5,E, C6,E, G5,E, E5,E
 .byte HUELL,5,C6,S
 .byte HUELL,4,G5,E, E5,E, PAUSE,S, PAUSE,E,ENDE

?shape_PACMEN_O5_UPr
 .BYTE HUELL,5
 .byte DIS5,S,E5,S
 .byte HUELL,4,F5,E
 .byte HUELL,5,F5,S,FIS5,S
 .byte HUELL,4,G5,  E
 .byte HUELL,5,G5,S,GIS5,S
 .byte HUELL,4,A5,  E
 .byte HUELL,5,A5,S,AIS5,S
 .byte HUELL,4,B5,E,ENDE


?shape_DRUMS
 .byte HUELL,2
 .byte 1,E,1,E,2,E,PAUSE,E, 1,E,1,E,2,E,PAUSE,E,ENDE

?shape_DRUMS_2
 .byte HUELL,2
 .byte 1,Q,2,Q,1,Q,2,Q,ENDE

?shape_BASS_B0
 .byte HUELL,1
; .byte ST_B1,Q,ST_B2,Q,ST_B1,Q,ST_B2,Q,ENDE
 .byte B1,Q,B2,Q,B1,Q,B2,Q,ENDE
; .byte B1,H,B2,Q,B1,Q,ENDE

?shape_BASS_C1
 .byte HUELL,1
; .byte ST_C2,Q,ST_C3,Q,ST_C2,Q,ST_C3,Q,ENDE
 .byte C2,Q,C3,Q,C2,Q,C3,Q,ENDE
; .byte C2,H,C3,Q,C2,Q,ENDE

?shape_BASS_B0_auslaufend
 .byte HUELL,1
; .byte ST_B1,Q
; .byte HUELL,6,ST_B2,H,ENDE
 .byte B1,Q
 .byte HUELL,6,B2,H,ENDE

?shape_BASS_GIS0
 .byte HUELL,1
; .byte ST_GIS1,Q,ST_GIS2,Q,ST_GIS1,Q,ST_GIS2,Q,ENDE
 .byte GIS1,Q,GIS2,Q,GIS1,Q,GIS2,Q,ENDE

?shape_BASS_A0
 .byte HUELL,1
; .byte ST_A1,Q,ST_A2,Q,ST_A1,Q,ST_A2,Q,ENDE
 .byte A1,Q,A2,Q,A1,Q,A2,Q,ENDE
; .byte A1,H,A2,Q,A1,Q,ENDE

?shape_BASS_AIS0
 .byte HUELL,1
; .byte ST_AIS1,Q,ST_AIS2,Q,ST_AIS1,Q,ST_AIS2,Q,ENDE
 .byte AIS1,Q,AIS2,Q,AIS1,Q,AIS2,Q,ENDE
; .byte AIS1,H,AIS2,Q,AIS1,Q,ENDE

?shape_BASS3_UP
 .byte HUELL,1
; .byte ST_FIS2,Q,ST_GIS2,Q,ST_AIS2,Q,ST_B2,Q,ENDE
 .byte FIS2,Q,GIS2,Q,AIS2,Q,B2,Q,ENDE

?shape_BASS3_DOWN
 .byte HUELL,1
; .byte ST_FIS2,Q,ST_E2,Q,ST_D2,Q,ST_CIS2,Q,ENDE
 .byte FIS2,Q,E2,Q,D2,Q,CIS2,Q,ENDE


?shape_DRUMS_3
 .byte HUELL,2
 .byte PAUSE,Q,PAUSE,Q,1,S,PAUSE,S,1,S,PAUSE,S,PAUSE,Q,ENDE



; Takte-List *********************************************-

@takt
?takt_array
;    .byte SPEED,1,0,0
;    .byte 47,0,0,0    ; Shape no up/down
;    .byte 45,0,0,0    ; tone up
;    .byte 46,0,0,0    ; tone down
;    .BYTE JUMP,0,0,0

;    .byte SPEED,1,0,0
;    .byte 16,40,0,0
;    .byte 17,0,0,0
;    .byte 16,0,0,0
;    .byte 18,30,25,0 ; BASS_DOWN
;    .BYTE JUMP,0,0,0

;    .byte 1,35,25,0
;    .byte 2,36,25,0
;    .byte 1,35,25,0
;    .byte 4,31,25,0 ; BASS_DOWN

;    .BYTE JUMP,0,0,0

;    .byte SPEED,1,0,0
;    .byte 0,35,25,0
;    .byte 0,30,25,0 ; BASS_UP
;    .byte 0,36,25,0
;    .byte 0,31,25,0 ; BASS_DOWN
;    .BYTE JUMP,0,0,0

;    .byte SPEED,1,0,0
;    .byte 1,35,25,0
;    .byte 2,36,25,0
;    .byte 1,35,25,0
;    .byte 4,31,25,0  ; down BASS_DOWN

;    .byte 0,34,25,0
;    .byte 0,35,25,0
;    .byte 0,34,25,0
;    .byte 0,30,25,0 ; BASS_UP

;    .byte 0,35,25,0
;    .byte 0,36,25,0
;    .byte 0,35,25,0
;    .byte 0,31,25,0 ; BASS_DOWN

;    .byte 6,34,25,0
;    .byte 7,35,25,0
;    .byte 6,34,25,0
;    .byte 8,30,25,0  ;up BASS_UP
;    .byte JUMP,1,0,0



;    .byte SPEED,0,0,0
;    .byte 0,0,0,0 ; 48 1/50s leer
;
;    .byte SPEED,4,0,0
;    .byte 41,0,0,0
;    .byte 42,0,0,0
;    .byte 41,0,0,0
;    .byte 43,0,0,0
;
;    .byte SPEED,0,0,0
;    .byte 0,0,0,0
;
;    .byte SPEED,3,0,0
;
;    .byte 1,0,0,0
;    .byte 2,0,0,0
;    .byte 1,0,0,0
;    .byte 3,0,0,0
;
;    .byte SPEED,3,0,0
;    .byte 0,25,35,0
;    .byte 0,25,36,0
;
;    .byte SPEED,2,0,0
;    .byte 0,25,35,0
;    .byte 0,25,36,0

;    .byte SPEED,1,0,0
;    .byte 0,35,25,0
;    .byte 0,36,25,0

;    .byte SPEED,1,0,0 ; Hi Speed
;    .byte 0,25,35,0
;    .byte 0,25,36,0

;    .byte 0,25,30,0
;    .byte 0,25,31,0
;    .byte 0,25,32,0
;    .byte 0,25,33,0
;    .byte 0,25,34,0
;    .byte 0,25,35,0
;    .byte 0,25,36,0
;    .byte 0,25,40,0

;    .byte 1,25,35,0
;    .byte 2,25,36,0
;    .byte 1,25,35,0
;    .byte 4,25,36,0 ; BASS_UP


;    .byte 61,0,35,0
;    .byte 62,0,36,0
;    .byte 63,0,35,0
;    .byte 64,0,36,0
;
;    .byte 65,0,35,0
;    .byte 66,0,36,0
;    .byte 67,0,35,0
;    .byte 68,0,36,0
;    .byte 0,25,30,0

;    .byte 51,0,35,0
;    .byte 52,0,35,0
;    .byte 53,0,35,0
;    .byte 54,0,35,0
;    .byte 55,0,35,0
;    .byte 56,0,35,0
;    .byte 57,0,35,0
;    .byte 58,0,35,0
;    .byte 0,25,30,0
;
;    .byte 41,0,35,0
;    .byte 42,0,35,0
;    .byte 43,0,35,0
;    .byte 44,0,35,0
;    .byte 45,0,35,0
;    .byte 46,0,35,0
;    .byte 47,0,35,0
;    .byte 48,0,35,0
;    .byte 0,25,31,0

;    .byte 41,0,35,0
;    .byte 42,0,36,0
;    .byte 43,0,35,0
;    .byte 44,0,36,0
;    .byte 45,0,35,0
;    .byte 46,0,36,0
;    .byte 47,0,35,0
;    .byte 48,0,36,0
;    .byte 0,0,31,0
;
;    .byte 41,0,35,0
;    .byte 42,0,36,0
;    .byte 43,0,35,0
;    .byte 44,0,36,0
;    .byte 45,0,35,0
;    .byte 46,0,36,0
;    .byte 47,0,35,0
;    .byte 48,0,36,0
;    .byte 0,0,36,0     ; up

;    .byte JUMP,3,0,0    ; Sprungweite in word (aber takt wird intern mit 4 multipliziert!)


    .byte SPEED,1,0,0 ; Hi Speed
    .byte 0,25,35,0
    .byte 0,25,36,0
;    .byte SPEED,1,0,0 ; Hi Speed
    .byte 0,25,35,0
    .byte 0,25,36,0
    .byte 0,25,35,0
    .byte 0,25,31,0

; Position 13
    .byte 6,25,35,0
    .byte 7,25,36,0
    .byte 6,25,35,0
    .byte 8,25,36,0 ; up

    .byte 1,25,35,0
    .byte 2,25,36,0

    .byte 1,25,35,0
    .byte 2,25,36,0
    .byte 1,25,35,0
    .byte 4,25,31,0 ; BASS_DOWN

    .byte 6,25,35,0
    .byte 7,25,36,0
    .byte 6,25,35,0
    .byte 9,25,31,0 ; down BASS_DOWN

    .byte 11,25,35,0
    .byte 12,25,36,0

    .byte 11,25,35,0
    .byte 12,25,36,0
    .byte 11,25,35,0
    .byte 13,25,30,0 ; up BASS_UP

    .byte 6,25,35,0
    .byte 7,25,36,0
    .byte 6,25,35,0
    .byte 8,25,31,0 ; up BASS_DOWN

    .byte 16,0,40,0
    .byte 17,0,0,0
    .byte 16,0,0,0
    .byte 18,25,30,0 ; BASS_DOWN

    .byte 1,25,35,0
    .byte 2,25,36,0
    .byte 1,25,35,0
    .byte 3,25,31,0 ; up BASS_DOWN

;    .byte 0,25,35,0
;    .byte 0,25,30,0 ; BASS_UP
;    .byte 0,25,36,0
;    .byte 0,25,31,0 ; BASS_DOWN

    .byte 1,25,35,0
    .byte 2,25,36,0

    .byte 6,25,35,0
    .byte 7,25,36,0

    .byte 11,25,35,0
    .byte 12,25,36,0

    .byte 6,25,35,0
    .byte 7,25,36,0

    .byte 1,25,35,0
    .byte 2,25,36,0

    .byte 6,25,35,0
    .byte 12,25,36,0
;    .byte 6,25,35,0
;    .byte 12,25,36,0
    .byte 1,25,35,0
    .byte 2,25,36,0
    .byte 1,25,35,0
    .byte 3,25,31,0 ; up BASS_DOWN

    .byte JUMP,3,0,0    ; Sprungweite in word (aber takt wird intern mit 4 multipliziert!)

