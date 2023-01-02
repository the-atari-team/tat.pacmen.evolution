;; -*- text -*-
;
; Pacmen Structure
;
; Hier alle Variablen ablegen, die ueberall gebraucht werden

 .local

; Wir verlegen die Pacmen-Structure in Page 5-7, da ist etwas Platz.

; sichern der aktuellen Adresse
?OLD_ADDRESS=*

 *=$0572 ; MEMORY_STRUCT

; curl "http://moonserver.my.to:8081/bannerVar?text=Game&height=9&comment=//"
;
;           OO                              OO
;  OOOOO  OOOOOOO OOOOOO  OO   OO  OOOOO  OOOOOOO OO   OO OOOOOO   OOOOO
; OO        OO    OO   OO OO   OO OO   OO   OO    OO   OO OO   OO OO   OO
;  OOOOO    OO    OO      OO   OO OO        OO    OO   OO OO      OOOOOOO
;      OO   OO    OO      OO   OO OO   OO   OO    OO   OO OO      OO
;  OOOOO     OOOO OO       OOOOOO  OOOOO     OOOO  OOOOOO OO       OOOOO
;

;
; OOOOOO
; OO   OO  OOOOOO  OOOOO  OOO OO   OOOOOO OOOOOO
; OO   OO OO   OO OO   OO OOOOOOO OO   OO OO   OO
; OOOOOO  OO   OO OO      OO O OO OO   OO OO   OO
; OO      OO   OO OO   OO OO O OO OO   OO OO   OO
; OO       OOOOOO  OOOOO  OO O OO  OOOOOO OO   OO
;

@PMM=$B300

; Position, wo Pacmen starten soll, wird beim Laden des PlayFields gesetzt
@pacmen_start_x
 .byte 0
@pacmen_start_y
 .byte 0

@pacmen_start_x_pf
 .byte 0
@pacmen_start_y_pf
 .byte 0

; Screen Position (4-148)
@pacmenx
 .byte 0

; Screen Position (8-152)
@pacmeny
 .byte 0

@pacmen_direction
 .byte 0

; wo befindet sich Pacmen innerhalb des playfields
@pacmen_xy_pf ; mul13[pacmeny] + pacmenx
 .byte 0

; Position innerhalb des Spielfeldes x (0-12)
@pacmen_x_pf
 .byte 0

; Position innerhalb des Spielfeldes y (0-6)
@pacmen_y_pf
 .byte 0

; Aktuelle Animation (Pointer auf die Daten)
@pacmen_animation
 .word 0

; Aktuelle Animation (Pointer innerhalb der Daten)
@pacmen_animation_number
 .byte 0

@pacmen_waitfor
 .byte 0

@pacmen_multiply_score_10
 .byte 0

@desired_level
 .byte 0

;  OOOOO  OO                        OO
; OO   OO OO       OOOOO   OOOOO  OOOOOOO  OOOOO
; OO      OOOOOO  OO   OO OO        OO    OO
; OO OOOO OO   OO OO   OO  OOOOO    OO     OOOOO
; OO   OO OO   OO OO   OO      OO   OO         OO
;  OOOOO  OO   OO  OOOOO   OOOOO     OOOO  OOOOO

; Die Start Position,
; wo ein Geist das erste mal erscheint,
; wird per load_background festgelegt
@ghost_start_x
 .byte 0,0,0,0

@ghost_start_y
 .byte 0,0,0,0

; Position die Ghost[number] anlaufen soll, wenn im Scatter-Mode
; Ecken, jeder Ghost hat seine eigene Ecke
@ghost_scatter_x
 .byte 13,0,0,13

@ghost_scatter_y
 .byte 7,7,1,1

; Screen Position (4-148)
@ghostx
 .byte 0,0,0,0

; Screen Position (8-152)
@ghosty
 .byte 0,0,0,0

; Welchen Mode nimmt ein Geist ein
; 1-CHASE Angriff
; 2-SCATTER
; 3-ANGST
@ghost_mode
 .byte 0,0,0,0

@ghost_mode_countdown
 .byte 0,0,0,0

; wie naehern sich die Geister dem Pacmen
; 1 - Direkte Position
; 2 - Front (px-2, py-2, px+2, py+2, je nach Pacmens laufrichtung...)
@ghost_approach
 .byte 0,0,0,0

; Pointer in der Tabelle fuer Zeiten von CHASE, SCATTER, ...
@ghost_chase_scatter_index
 .byte 0,0,0,0

; wo befinden sich die Geister innerhalb des playfields
@ghost_xy_pf
 .byte 0,0,0,0

; Ghost waitcount, startet bei 0 und wird jede 1f hochgezaehlt
; wird waitfor ueberschritten, gibt es 1f (1/50s) pause (Stotterbremse)
@ghost_waitcount
 .byte 0,0,0,0

; wie haeufig soll die Stotterbremse greifen 1-60)
@ghost_waitfor
 .byte 0,0,0,0

; wieviele Frames wollen wir den Start verhindern (0 bis 254) bis ~5s
@ghost_start_movement
 .byte 0,0,0,0

; Geist wird unsichtbar, wenn visible auf 0
@ghost_visible
 .byte 1,1,1,1

; TODO: speed of Ghost (1,2,3,4) fuer Highspeed
;@ghostnextspeed
; .byte 0,0,0,0

@ghost_animation_number
 .byte 0,0,0,0

@ghost_animation
@ghost_animation_low
 .byte 0,0,0,0
@ghost_animation_high
 .byte 0,0,0,0

@ghost_direction
 .byte 0,0,0,0

@ghost_pupils
 .byte 0,0,0,0

@ghost_move_direction
 .byte 0,0,0,0

@ghost_color
 .byte 0 ; $34 ; red
 .byte 0 ; $86 ; blue besser $18
 .byte 0 ; $b8 ; green
 .byte 0 ; $6a ; pink

; zeigt, welchen Score wir bekommen wenn ein Geist gegessen wird 200,400,800 oder 1600
@ghost_killed_for_score_index
 .byte 0

@ghosts_killed
 .word 0

;
;  OOOOO   OOOOO  OO   OO OOOOOOO
; OO   OO OO   OO OOO OOO OO
; OO      OO   OO OOOOOOO OO
; OO      OO   OO OO O OO OOOOO
; OO OOOO OO   OO OO   OO OO
; OO   OO OOOOOOO OO   OO OO
;  OOOOO  OO   OO OO   OO OOOOOOO
;

@player_lives
 .byte 0

@game_difficult
 .byte 0

@game_level
 .byte 0

; Game - Mode
; 1:= title
; 2:= game
@game_mode
 .byte 0

; Anzahl der Cookies auf dem Playfield, wird bei load_background() gesetzt
@countOfCookies
 .byte 0

; Steht auf 1, nur dann kann an der Stelle vom Pacmen ein collectible erscheinen
; ist in EXTRA Level grundsaetzlich auf 0 gesetzt
@newCollectiblePossible
 .byte 0

@newCollectibleCountdown
 .word 0

@newCollectibleWaitfor
 .word 0

@currentCollectible
 .byte 0

@eatenCollectible
 .byte 0


; Anzahl der Super-Cookies auf dem Playfield, wird bei load_background() gesetzt
@countOfSuperCookies
 .byte 0

@drawableChbas
 .byte 0

@playfield_color
  .byte 0,0,0,0,0

; 13 x 7 playfield
@playfield
 *=*+91

@colbk_change
 .byte 0

@debug
 .byte 0

;
; OOOOOO   OOOOO   OOOOO  OO   OO  OOOOO  OO   OO        OOOOOO  OO     OO  OOOOO OOOOOO
; OO   OO OO   OO OO   OO OOO OOO OO   OO OOO  OO        OO   OO OO     OO OO       OO
; OO   OO OO   OO OO      OOOOOOO OO   OO OOOO OO OOOOOO OO   OO OO     OO  OOOOO   OO
; OOOOOO  OO   OO OO      OO O OO OO   OO OO OOOO        OO   OO OO     OO      OO  OO
; OO      OOOOOOO OO   OO OO   OO OOOOOOO OO  OOO        OO   OO OO     OO      OO  OO
; OO      OO   OO  OOOOO  OO   OO OO   OO OO   OO        OOOOOO  OOOOOO OO  OOOOO   OO
;

;
; Displaylist fuer Pacmen
;
@game_screen = $bba0

@pacmen_title_screen = $bd08 ; bd30-40

@pacmen_title_text=$bf60

@PACMEN_GAME_DLIST
 .byte DL_E8
 .byte DL_E5|DL_DLI         ; DLI: ?CHANGE_ZSATZ_TITLE_PACMEN_DLI
 .byte DL_E1
 .byte DL_GR12|DL_ADDRES
 .word @pacmen_evolution_line

 .byte DL_E1|DL_DLI         ; DLI: ?CHANGE_ZSATZ_DLI
 .byte DL_E1

 .byte DL_GR12|DL_ADDRES
 .word game_screen
 .byte DL_GR12, DL_GR12, DL_GR12, DL_GR12, DL_GR12, DL_GR12, DL_GR12, DL_GR12
 .byte DL_GR12, DL_GR12, DL_GR12, DL_GR12, DL_GR12, DL_GR12, DL_GR12, DL_GR12
 .byte DL_GR12, DL_GR12, DL_GR12, DL_GR12
 .byte DL_GR12|DL_DLI       ; DLI: PACMEN FONT 2
 .byte DL_E1                ; So wie es aktuell ausschaut wird diese Leerzeile dringend gebraucht
 .byte DL_GR1, DL_GR1
 .byte DL_LOOP
 .word @PACMEN_GAME_DLIST



@PACMEN_TITLE_DLIST
 .byte DL_E8
 .byte DL_E8
 .byte DL_E6|DL_DLI           ; DLI:  ?CHANGE_ZSATZ_TITLE_PACMEN_DLI
 .byte DL_E2

 .byte DL_GR13|DL_ADDRES      ; Pacmen Evolution Graphics 13
 .word @pacmen_evolution_line

 .byte DL_E8

 .byte DL_E1|DL_DLI           ; DLI:  ?CHANGE_ZSATZ_TITLE_DLI
 .byte DL_E1

 .byte DL_GR1|DL_ADDRES       ; by the atari team
 .word @pacmen_title_text
 .byte DL_E2

 .byte DL_GR1, DL_E2          ; cdw 2022
 .byte DL_GR1, DL_E2          ; press fire to start
 .byte DL_GR1, DL_E2          ; text

 .byte DL_E1|DL_DLI           ; DLI:  ?CHANGE_ZSATZ_GAME_SUBSET_DLI
 .byte DL_E3

 .byte DL_E2

 .byte DL_GR12|DL_ADDRES      ; 10 Zeilen Game field fuer Title
 .word @pacmen_title_screen
 .byte DL_GR12
 .byte DL_GR12
 .byte DL_GR12
 .byte DL_GR12
 .byte DL_GR12
 .byte DL_GR12
 .byte DL_GR12
 .byte DL_GR12
 .byte DL_GR12

 .byte DL_E1|DL_DLI           ; DLI:  ?CHANGE_ZSATZ_TITLE_OTHER_DLI
 .byte DL_E3
 .byte DL_E2

 .byte DL_GR1|DL_ADDRES       ; text
 .word @pacmen_title_text+80  ; 4 Zeilen weiter
 .byte DL_E2
 .byte DL_GR1, DL_E2          ; text
 .byte DL_GR1, DL_E2          ; text
 .byte DL_GR1, DL_E2          ; text

 .byte DL_LOOP
 .word @PACMEN_TITLE_DLIST

@pacmen_evolution_line
; GR12/GR13 Line fuer " PAC MeN-evolution"
; Char-Position in pacmen-font.wnf
 .byte 90,86,87,88,89 ; zvwxy
; .sbyte " abcdef ghcdij - " ; klmnopqrstu yzvwxy"
; PAC MAN Schriftzug
; .byte 0,97,98,99,100,101,102,0,103,104,99,100,105,106,0,13,0
; PAC MeN Schriftzug
 .byte 92,93,94
 .byte 97,98,99,100,101,102,103,104,95,96,105,106
 .byte 0,13,0
; evolution
 .byte 75,76,77,78,79,80,81,82,83,84,85
 .byte 89,90,86,87,88,89

  .if * >= $6fa
    .error "Speicher ueberlauf! * must < $6fa"
  .endif

; wiederherstellen der aktuellen Adresse
 *=?OLD_ADDRESS

; Extrem Speicherintensiv
; durch Floyd-Warshall berechnet (O(n^3))
; Nicht im Wechsel schaffbar und kostbare 8kb teuer
;@successors
;  *=*+8281
