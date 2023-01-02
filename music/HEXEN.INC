;*********************************************
;*               MUSIK IM VBI                *
;*                                           *
;* Musikprogramm mit Huellkurvengenerator    *
;*                                           *
;* Spielt bis zu 3 Noten gleichzeitig        *
;* 1,2 8bit Version,
;*     timbre frei waehlbar (0,2,4,6,8,10,12,14)
;*     huellkurve fuer Lautstaerke
;* 3   16bit Version dadurch klarer Bass
;*     timbre frei waehlbar
;*     huellkurve fuer Lautstaerke
;*
;* 16-bit:  AUDCTL=$40+$10, or $20+$08.
;  Sets 1.79mhz on first channel.
;   AUDC1/AUDC3=$00, AUDC2/AUDC4=$xx (first channel silent)
;
;*********************************************
;
; TODO: Umbau, toene mussen lange spielen! >=64 oder >=128 mit ganz langsam ausklingen
; TODO: Shape spielt letzten Ton auch ueber Grenze hinweg.
;
;*********************************************

  .local

;**************************************************
; Definition Noten/Frequenz und Notendauer
; liegt in der Datei "NOTEN.INC"
;**************************************************
; ACHTUNG: Diese 6 Bytes duerfen wo anders nicht verwendet werden.
music_DATA = 203     ; 2
music_POINTER = 205  ; 2
music_FREQ = 207     ; 1
music_AUDC = 208     ; 1

?produkt=209         ; 2
?op=211              ; 1

; So sind wir in der Lage uns vor einen schon vorhandenen Interrupt zu setzen
?OLD_ADDRESS=*

 *=$06fe
remember_VVBLKD
  .byte 0
remember_VVBLKD_1
  .byte 0

 *=?OLD_ADDRESS
;
; Restore old Interrupt and remove it

@music_restore_Interrupts
      jsr @music_stop_timer

      lda #0
      sta music_INIT_NO_VBI

      ldx remember_VVBLKD_1
      beq ?empty_remembervalues ; --> 0 dann ist das HighByte 0, raus

      ldy remember_VVBLKD
      lda #7
      jmp setvbv

?empty_remembervalues
      rts


; Initialise 2 Interrupts for HEXEN Player
; 1. Deferred Interrupts plays shapes
;   Init will use an already set Deferred Interrupt and jump to this exit address
;
; 2. Timer Interrupt 2 plays Takte
;   Will only set, not start

@music_init_interrupts

  jsr @music_INIT_TAKT_Interrupt
  jmp @music_INIT_VBI_Interrupt


@music_INIT_TAKT_Interrupt
      jsr @music_stop_timer

      LDA #0
      sta music_INIT_NO_VBI

      LDY # <TAKTVBI      ; Sound einschalten
      LDX # >TAKTVBI
      LDA #9              ; Timer 2 Adresse setzen
      JMP SETVBV


@music_INIT_VBI_Interrupt

;      LDY #2              ; counter starten
;      LDX #0
;      LDA #2
;      JSR SETVBV

      LDA VVBLKD
      LDY VVBLKD+1
      STA remember_VVBLKD
      STY remember_VVBLKD_1

      LDY   #<MUSICVBI
      LDX   #>MUSICVBI
      LDA   #7            ;  fuer Deferred VBI
      JMP   SETVBV        ;  O.S.-Routine, VBI setzen >>


;
; OO    OO OOOOOOO   OOOOOO
; OO    OO OO    OO    OO
; OO    OO OO    OO    OO
; OO    OO OOOOOOO     OO
; OO    OO OO    OO    OO
;  OO  OO  OO    OO    OO
;   OOOO   OOOOOOO   OOOOOO
;
; Im getakteten Interrupt bearbeiten wir nur die Takte
; So kann ein Musikstueck sauber durch Poke 538,0 beendet werden
TAKTVBI
      cld
      LDA #1
      STA CDTMV2                ;counter starten!

      jmp @music_takt_vbi

; Im VBI spielen wir Shapes ab
MUSICVBI
      CLD
      jsr @music_play_vbi
      JMP (remember_VVBLKD)    ; dann VBI beenden ==>
;      jmp xitvbv

; muss alle 1/50s angesprungen werden, sollen weiter Takte durchgespielt werden
@music_takt_vbi
      LDA MUSIC_INIT_NO_VBI
      Beq ?NO_TAKT_PLAY
      JMP Music_Play_Takt

?NO_TAKT_PLAY
?NO_MUSIC_PLAY
      RTS


; muss alle 1/50s angesprungen werden.
@music_play_vbi
      LDA MUSIC_INIT_NO_VBI
      Beq ?NO_MUSIC_PLAY

      JMP MUSIC_PLAY


;**************************************************
; Interne Variablen fuer VBIMUSIK
;**************************************************

music_audf
      .byte 0,0,0,0       ; Frequenzwert fuer AUDFn

Music_timbre              ; diesen Timbre lesen wir aus der Huellkurve
      .byte 0,0,0,0       ; wird auch fuer die Frequenzen gebraucht (BASS, BASS2)

music_real_timbre     ; diesen Timbre schreiben wir dann in AUDCn
      .byte $00   ; effekt
      .byte $20   ; effekt
      .byte $40   ; effekt
      .byte $20   ; effekt $60 ist wie $20
      .byte $80   ; DRUMS
      .byte $A0   ; Clean
      .byte $C0   ; BASS1
      .byte $C0   ; BASS2
;     .byte $E0   ; Clean, wird nicht verwendet

Music_Note_Dauer          ; Zaehler fuer Tondauer
      .byte 0,0,0,0

music_note
      .byte 0,0,0,0      ; aktuelle Note, wird erst in Huellkurve konvertiert

Music_Pause              ; Flag fuer Pause
      .byte 0,0,0,0

Music_in_Huellkurve_Pointer ; Zeiger in Huellkurve
      .byte 0,0,0,0

; Huellkurve, die gerade verwendet wird
Music_Huellkurve_low
      .byte 0,0,0,0

Music_Huellkurve_high
      .byte 0,0,0,0

; Takt laeuft immer syncron, deshalb nur einer vorhanden
Music_Takt_Dauer
      .byte 0

Music_in_Takt_Pointer
      .word 0

Music_current_shape
      .byte 0,0,0,0

Music_in_Shape_Pointer
      .byte 0,0,0,0

Music_Shape_Pointer_low
      .byte 0,0,0,0

Music_Shape_Pointer_high
      .byte 0,0,0,0

music_save_x
      .byte 0

music_save_y
      .byte 0

MUSIC_INIT_NO_VBI
      .byte 0

; ist der Wert 1, kann eine Vibration gespielt werden, bei 0 keine weitere Vibration moeglich.
; Damit wird unter/ueberlauf der Noten gesteuert
?music_is_vibra_possible
      .byte 1,1,1,1

?music_current_vibration_low
      .byte 0,0,0,0

?music_current_vibration_high
      .byte 0,0,0,0

?music_audctl
  .byte 0

;
;   OO                                         OOO                                  OO   OO
;   OO                                          OO                                  OO   OO
; OOOOOO  OOOOOOO  OOOOOOOO   OOOOOOO           OO   OOOOOOO  OOOOOOOO   OOOOOOOO OOOOOO OOOOOOOO
;   OO   OO     OO OO     OO OO     OO          OO  OO     OO OO     OO OO     OO   OO   OO     OO
;   OO   OO     OO OO     OO OOOOOOOOO          OO  OOOOOOOOO OO     OO OO     OO   OO   OO     OO
;   OO   OO     OO OO     OO OO                 OO  OO        OO     OO  OOOOOOOO   OO   OO     OO
;    OOO  OOOOOOO  OO     OO  OOOOOOO          OOOO  OOOOOOO  OO     OO        OO    OOO OO     OO
;                                                                       OOOOOOOO

; Laenge einer Note in 1/50s (1/60s)
; Voll(F), halb(H), viertel(Q), achtel(E), 16tel(S)
@PossibleSpeedList
       .byte  48,24,12, 6, 3,1,1,0   ; 0=fastest
       .byte  64,32,16, 8, 4,2,1,0   ; 1=faster
       .byte  80,40,20,10, 5,2,1,0   ; 2=fast
       .byte  96,48,24,12, 6,3,1,0   ; 3=normal PAL Voll, halb, viertel, achtel, 16tel, 32tel, 50tel
       .byte 112,56,28,14, 7,3,1,0   ; 4=normal NTSC
       .byte 128,64,32,16, 8,4,2,1   ; 5=slow
       .byte 144,72,36,18, 9,5,3,1   ; 6=slower
       .byte 160,80,40,20,10,5,3,1   ; 7=slowest

; Music Speed wird gesetzt
; @music_set_speed(n) n ist [0-7] moeglich 0, schnell, 7 langsam
; n=3 ist normal speed bei PAL  Ein Takt ca. 2s
; n=4 ist normal speed bei NTSC Ein Takt ca. 2s
@music_set_speed
@music_set_speed_i
  ldy #1
  lda (@heap_ptr),y

; wir erwarten den zu setzenden Speed im Akku
; MUSIC_DATA muss gesetzt sein und in die Musik Daten zeigen
music_set_speed
    and #7                          ; Stimmt nicht ganz
    asl
    asl
    asl
    tax
    ldy #8
?setspeedloop
    lda @PossibleSpeedList,x
    sta (MUSIC_DATA),y
    inx
    iny
    cpy #16
    bne ?setspeedloop
    rts

;**************************************************
; Musik play, alle 1/50sec. aufrufen!
;**************************************************

MUSIC_PLAY

;       JSR Music_Play_Takt

       LDX #0                      ; Kanal 0 zuerst spielen
play_loop
       JSR Music_Play_Shape_X      ; in music_audf,x sollte Tone stehen

       JSR Music_Play_Huellkurve_X ; Huellkurve f. eine Stimme >>
       INX                         ; naechste Stimme

       CPX #3                      ; wir spielen nur noch 3 Noten (letzte in 16bit)
       BNE play_loop

       lda ?music_audctl
       and #AUDCTL_34_16bit
       bne ?AUDCTL_IS_16BIT        ; --> 16 bit gesetzt

       ldx #3
       JSR Music_Play_Shape_X
       JSR Music_Play_Huellkurve_X

?AUDCTL_IS_16BIT
       RTS
;
;?parameter .word 0,0


;**************************************************
; HEXEN-Player-Initialisierungsroutine
; POKEY init., Variablen loeschen, Takt setzen
;
; Umsetzen der Audio Daten
; Es werden alle Kanaele initialisiert
; @music_init(adr:MusicTable, AUDCTL, adr:TaktTable)
; Adresse zur 4 Word Liste,
; Wert auf AUDCTL, meist 0 (4 8bit Kanaele oder $28 2 8bit 1 16bit Kanal)
; Adresse zur Takt Liste
;
; Um die Musik zu starten muss noch @music_start_timer() gerufen werden
;**************************************************
@music_init
@music_init_iii
  ldy #1
  lda (@heap_ptr),y
  sta music_data
  iny
  lda (@heap_ptr),y
  sta music_data+1

  LDA #3              ; Pokey ruecksetzen
  STA SKCTL
  LDA #0
  STA AUDCTL

  iny
  lda (@heap_ptr),y
;  LDA   #$28         ; Sound-Kontrollreg. zurueck
                      ; $08 Kanal 3&4 zusammenschalten 16bit
                      ; $20 Kanal 3 auf 1,79MHz
  STA AUDCTL
  sta ?music_audctl

  ldy #6
  lda (@heap_ptr),y      ; high vom Takt
  tax                    ; zwischenspeichern
  dey
  lda (@heap_ptr),y      ; low Takt

  ldy #6                 ; hier beginnt der Takt
  sta (MUSIC_DATA),Y     ; low
  iny
  txa
  sta (MUSIC_DATA),Y     ; high

; Musik still stellen
@music_silent

  LDA #0                  ; sollte jetzt ein VBI dazwischen funkten, nichts machen
  STA MUSIC_INIT_NO_VBI

  lda #3                    ; PAL Normal Speed!
  jsr music_set_speed

  LDA #0
  sta Music_Takt_Dauer
  sta Music_in_Takt_Pointer
  sta Music_in_Takt_Pointer+1

  LDX #3

?MUSIC_init_loop
  jsr music_init_X
  DEX
  BPL ?MUSIC_init_loop

  LDA #1
  STA MUSIC_INIT_NO_VBI
  rts

; Wir initialisieren nur einen Shape
@music_init_single_shape
@music_init_single_shape_i
  ldy #1
  lda (@heap_ptr),y
  and #~00000011
  tax

; if cpx == 2 and audctl ~= 16bit
  cpx #2
  bne music_init_x

  lda ?music_audctl
  and #AUDCTL_34_16bit
  bne music_init_16bit      ; --> 16 bit gesetzt

; Initialisiere nur einen Kanal in X-Register
music_init_X
  lda #0

  STA AUDF1,X
  STA AUDF1+4,X
  STA Music_Note_Dauer,X

  sta Music_Shape_Pointer_low,x
  sta music_shape_pointer_high,x

  ; wir erlauben Huellkurven bis 255 bytes, so koennen wir auch Geraeusche abspielen
  STA Music_in_Huellkurve_Pointer,X   ; Huellk.-Zeiger:=Tabellenanfang
  sta music_huellkurve_low,x
  sta music_huellkurve_high,x
  RTS

music_init_16bit     ; kleine Hilfsroutine um 2 Kanaele zu initialisieren
  ldx #2
  jsr music_init_X
  ldx #3
  jmp music_init_X

  RTS


; mit dem 2. Interrupt wird der Taktgeber wieder gestartet
@music_start_timer
  lda CDTMA2
  ora CDTMA2+1
  beq ?second_timer_not_initialised_do_nothing

  LDA #1
  STA CDTMV2
?second_timer_not_initialised_do_nothing
  RTS


; Wird der Takt Interrupt gestoppt, wird der aktuelle Shape zu Ende gespielt
@music_stop_timer
  LDA #0
  sta CDTMV2+1
  STA CDTMV2
  RTS

;
;           OOO                                OO             OO        OO
;            OO                                OO             OO        OO
; OOOOOOOO   OO   OOOOOOOO OO     OO         OOOOOO  OOOOOOOO OO  OO  OOOOOO
; OO     OO  OO  OO     OO OO     OO           OO   OO     OO OO OO     OO
; OO     OO  OO  OO     OO OO     OO           OO   OO     OO OOOOO     OO
; OOOOOOOO   OO  OO     OO  OOOOOOOO           OO   OO     OO OO  OO    OO
; OO        OOOO  OOOOOOOO        OO            OOO  OOOOOOOO OO   OO    OOO
; OO                       OOOOOOOO


;**************************************************
;Einen Takt spielen
;**************************************************
; TODO: music_init_takt ist noch nicht fertig
@music_init_takt
  rts

Music_Play_Takt
       LDA Music_Takt_Dauer
       BEQ music_Takt_NEU

       DEC Music_Takt_Dauer
       BNE music_Play_Takt_Ende

music_Takt_NEU
       LDA Music_in_Takt_Pointer+1
       BNE music_Takt_already_defined

set_to_first_takt
       jsr reset_to_first_takt

music_Takt_already_defined
       lda Music_in_Takt_Pointer
       sta MUSIC_POINTER
       lda Music_in_Takt_Pointer+1
       sta MUSIC_POINTER+1

       ldy #0
       lda (MUSIC_POINTER),y

; Pruefen, war es ein Command
       cmp #START
       beq set_to_first_takt

       cmp #ENDE
       beq music_Play_Takt_command_ENDE

; TODO: jsr fuer wiederkehrende Dinge?
; TODO: Schleifen fuer wiederkehrende Dinge?

       cmp #JUMP
       beq music_play_takt_command_JUMP

       cmp #AUDIO_INIT
       beq music_play_takt_command_AUDIO_INIT

init_shape_loop
       lda (MUSIC_POINTER),y     ; all Shapes aus Takt-List setzen
       sta Music_current_shape,y
       iny
       cpy #4
       bne init_shape_loop

       ldy #8                    ; Music Speed setzen
       lda (MUSIC_DATA),y
       sta Music_takt_dauer

       ldy #4                    ; Shapes umrechnen aus Shape-Daten setzen
       LDA (MUSIC_DATA),Y
       STA MUSIC_POINTER
       INY
       LDA (MUSIC_DATA),Y
       STA MUSIC_POINTER+1

       ldx #0
load_shape_loop
       jsr init_single_shape_x
       inx
       cpx #4
       bne load_shape_loop

; einen Takt weiter, (4 byte)
;       jmp goto_next_takt  ; small optimize, do not jump

goto_next_takt
       clc
       lda Music_in_Takt_Pointer
       adc #4
       sta Music_in_Takt_Pointer
       bcc ?no_inc
       inc Music_in_Takt_Pointer+1
?no_inc

music_Play_Takt_Ende
       RTS


init_single_shape_x
       lda Music_current_shape,x
       asl a
       tay
       lda (MUSIC_POINTER),y
       sta Music_Shape_Pointer_low,x
       iny
       lda (MUSIC_POINTER),y
       sta Music_Shape_Pointer_high,x

?inner_init_single_shape_x
       lda #0
       STA Music_in_Shape_Pointer,x
       sta music_note_dauer,x        ; Shape auf Anfang, alte Note wird gekillt, falls die noch laeuft

       rts


; Ende erreicht, wir setzen einen Takt, allerdings alle Shapes auf 0
music_Play_Takt_command_ENDE
       ldy #8
       lda (MUSIC_DATA),y
       sta Music_takt_dauer
       lda #0
       tay
no_shape_loop
       sta Music_current_shape,y
       sta Music_Shape_Pointer_low,y
       sta Music_Shape_Pointer_high,y
       iny
       cpy #4
       bne no_shape_loop
       RTS


music_play_takt_command_AUDIO_INIT
       iny
       lda (MUSIC_POINTER),y  ; get speed
       tax                    ; save speed

       iny
       lda (MUSIC_POINTER),y  ; get new AUDCTL
       sta AUDCTL

       txa
       jsr music_set_speed

       jsr goto_next_takt
       jmp music_Takt_already_defined


music_play_takt_command_JUMP
       iny
       lda (MUSIC_POINTER),y  ; Sprungweite
       sta sprungweite
       iny
       lda (MUSIC_POINTER),y
;       sta sprungweite+1

       asl sprungweite        ; * 4
       rol a                  ; sprungweite+1
       asl sprungweite
       rol a                  ; sprungweite+1
       sta sprungweite+1

       jsr reset_to_first_takt

       clc
       lda Music_in_Takt_Pointer
       adc sprungweite
       sta Music_in_Takt_Pointer
       lda Music_in_Takt_Pointer+1
       adc sprungweite+1
       sta Music_in_Takt_Pointer+1
       jmp music_Takt_already_defined

sprungweite .word 0

reset_to_first_takt
       ldy #6                 ; ersten Takt laden
       LDA (MUSIC_DATA),Y
       STA Music_in_Takt_Pointer
       INY
       LDA (MUSIC_DATA),Y
       STA Music_in_Takt_Pointer+1
       rts

; play a single shape on canal n
; @music_play_single_shape(canal, shape)
@music_play_single_shape
@music_play_single_shape_ii
  ldy #1
  lda (@heap_ptr),y
  and #~00000011     ; only 0-3 moeglich
  tax

  ldy #3
  lda (@heap_ptr),y

  sta music_current_shape,x

  ldy #4                    ; Shapes umrechnen aus Shape-Daten setzen
  LDA (MUSIC_DATA),Y
  STA MUSIC_POINTER
  INY
  LDA (MUSIC_DATA),Y
  STA MUSIC_POINTER+1

  jmp init_single_shape_x

; @music_count_shapes_play()
; @return how many shapes play a tone, 0 if none (still)
@music_count_shapes_play
  lda #0
  sta ?count_of_playing_shapes

  ldx #3
?wait_loop
  jsr music_wait_shape_X
  bcc ?wait_loop_next
  inc ?count_of_playing_shapes

?wait_loop_next
  dex
  bpl ?wait_loop

  ldy ?count_of_playing_shapes
  ldx #0
  rts

music_wait_shape_X
  lda music_note_dauer,x
  beq ?shape_is_still
  sec
  rts

?shape_is_still
  clc
  rts

?count_of_playing_shapes
  .byte 0

;
;           OOO                                        OO
;            OO                                        OO
; OOOOOOOO   OO   OOOOOOOO OO     OO          OOOOOOO  OOOOOOOO   OOOOOOOO OOOOOOOO   OOOOOOO
; OO     OO  OO  OO     OO OO     OO         OO        OO     OO OO     OO OO     OO OO     OO
; OO     OO  OO  OO     OO OO     OO          OOOOOOO  OO     OO OO     OO OO     OO OOOOOOOOO
; OOOOOOOO   OO  OO     OO  OOOOOOOO                OO OO     OO OO     OO OOOOOOOO  OO
; OO        OOOO  OOOOOOOO        OO          OOOOOOO  OO     OO  OOOOOOOO OO         OOOOOOO
; OO                       OOOOOOOO                                        OO

?music_shape_x_restart
  jsr ?inner_init_single_shape_x
  tay
  jmp ?music_play_shape_x_again

;**************************************************
;Einen Kanal spielen
;pruefen der Tondauer, evtl. neue Note besorgen,
;Test auf ENDE, START, PAUSE
;**************************************************
;
Music_Play_Shape_X
       LDA Music_Note_Dauer,X       ; ist Kanal noch aktiv?
       BEQ Music_play_shape_x_new   ; nein --> dann neue Note

       DEC Music_Note_Dauer,X       ; sonst nur Tondauer - 1
       beq Music_play_shape_x_new

       rts                          ; aktuelle Huellkurve laeuft noch --->

; Erkennen, welche Note gespielt werden soll
Music_play_shape_x_new
       LDA #0                       ; Flag fuer Pause vorerst
       STA Music_Pause,X            ; zuruecknehmen

music_next_note
       LDA Music_Shape_Pointer_low,X
       STA MUSIC_POINTER
       LDA Music_Shape_Pointer_high,X
       STA MUSIC_POINTER+1

       lda MUSIC_POINTER            ; pruefen, ob Shape 0
       ora MUSIC_POINTER+1
       beq music_shape_x_pause      ; --> ja: nichts abspielen

       LDY Music_in_Shape_Pointer,X ; Zeiger in Notentabelle
?music_play_shape_x_again
       LDA (MUSIC_POINTER),Y        ; Note oder command

; Pruefen, war es ein Command

; music_nicht_neu_starten
       CMP #ENDE                    ; ist ENDE-Befehl?
       BEQ music_shape_x_pause      ; ja, dann Pause imitieren -->

       CMP #START
       BEQ ?music_shape_x_restart

       CMP #HUELL
       BEQ huellkurve_change

       STA MUSIC_FREQ

       CMP #PAUSE                   ; bei Pause spielt nur huellkurve weiter
       BNE No_pause

; wir sind pause, spielt die letzte Note einfach weiter
       jmp storeDauerUpdateNotePointer
;       JMP music_shape_x_play_ende

No_pause
       LDA #0
       STA Music_in_Huellkurve_Pointer,X   ; Huellk. Zeiger auf Anf.

; Kein Command
       jsr storeDauerUpdateNotePointer

       LDA MUSIC_FREQ
       beq music_shape_x_pause   ; sonderfall 0, spiele nichts

       sta music_note,x
       lda #0
       sta ?music_current_vibration_low,x
       sta ?music_current_vibration_high,x
       rts

;       CMP #PAUSE                ; war es ein PAUSE Befehl?
;       BEQ music_shape_x_pause   ; ja ,dann Pausenbearbeitung

; Da wir jetzt Zeiger speichern, die auf eine neue Tabelle zeigen
;       jsr music_convertNoteZuAtariTone
;music_shape_x_play_ende
;       RTS

; Macht aus einer Note(Akku) einen Atari Tone
; in Abhaengigkeit von der Verzerrung (Disortion oder Timbrel)
; in music_freq muss die gewuenschte Note stehen
music_convertNoteZuAtariTone

; TODO: Das funzt nur mit clean
       stx music_save_x

       lda music_timbre,x
       and #~11100000

       cmp #CLEAN              ; $a0
       beq timbre_clean

       cmp #BASS1              ; $c0
       beq timbre_12a

       cmp #BASS2              ; $e0 G#0-
       beq timbre_12b

       lda music_freq          ; keine Anpassung noetig
       jmp set_audf_tone

timbre_12a
       ldx music_freq
       lda NTSC_timbre_buzzy_DISCa,x
       jmp set_audf_tone

timbre_12b
       ldx music_freq
       lda NTSC_timbre_smooth_DISCb,x
       jmp set_audf_tone

; TODO: wir brauchen ein Flag, das uns sagt 16bit oder 8bit
timbre_clean
       cpx #2
       bne get8bit

       lda ?music_audctl
       and #audctl_34_16bit
       bne get16bit

get8bit
       ldx music_freq
       lda @PAL_64k_DISA,x

set_audf_tone
       ldx music_save_x
       STA music_AUDF,x              ; neue Note an POKEY
       lda #1
       sta ?music_is_vibra_possible,X ; Vibration moeglich

       RTS

music_shape_x_pause
       LDA #PAUSE             ; Huellk. ausschalten
       STA Music_Pause,X
       RTS

get16bit
       ldx music_freq
       lda @PAL_1_79m_DISA_low,x
       STA music_AUDF+2            ; sonst Note an POKEY
       lda @PAL_1_79m_DISA_high,x
       STA music_AUDF+3
       ldx music_save_x
       rts


; bis zu 121 Huellkurven koennen wir unterstuetzen
?music_save_y = 209
?new_huellkurve = 210

huellkurve_change
       INY                    ; Huellkurve aus Notentabelle lesen
       STY ?music_save_y

       LDA (MUSIC_POINTER),Y
       ASL
       STA ?new_huellkurve

       LDY #2                 ; Huellkurven Liste setzen
       LDA (MUSIC_DATA),Y
       STA MUSIC_POINTER
       INY
       LDA (MUSIC_DATA),Y
       STA MUSIC_POINTER+1

       ldy ?new_huellkurve

       lda (MUSIC_POINTER),y
       STA music_huellkurve_low,X
       iny
       lda (MUSIC_POINTER),y
       STA music_huellkurve_high,X

; nachdem die Huellkurve initialisiert wurde, gleich den
; ersten Timbre Wert auslesen
       sta music_pointer+1
       lda music_huellkurve_low,x
       sta music_pointer

       ldy #0
       lda (music_pointer),y  ; Timbre Wert auslesen
       and #$F0
       sta music_timbre,x

       LDY ?music_save_y

next_note
       INY                    ; Noten Zeiger auf
       TYA                    ; naechste Note richten
       STA Music_in_Shape_Pointer,X ; und abspeichern
       JMP music_next_note

storeDauerUpdateNotePointer
       INY                    ; Dauer aus Notentabelle lesen
       STY ?music_save_y

       LDA (MUSIC_POINTER),Y  ; get Dauer
       CLC
       ADC #8
       TAY
       LDA (MUSIC_DATA),Y     ; konvertiere Dauer in 50stel

       STA Music_Note_Dauer,X ; in Zaehler f. Notendauer

       LDY ?music_save_y
       INY                    ; Noten Zeiger auf
       TYA                    ; naechste Note richten
       STA Music_in_Shape_Pointer,X ; und abspeichern

music_X_Huellkurve_ENDE
       RTS

;
; OO                  OOO  OOO
; OO                   OO   OO
; OOOOOOOO  OO     OO  OO   OO           OOOOOOO  OO     OO OOOOOOOO  OO   OO  OOOOOOO
; OO     OO OO     OO  OO   OO          OO     OO OO     OO OO     OO OO   OO OO     OO
; OO     OO OO     OO  OO   OO          OO        OO     OO OO        OO   OO OOOOOOOOO
; OO     OO OO     OO  OO   OO          OO     OO OO     OO OO         OO OO  OO
; OO     OO  OOOOOOOO OOOO OOOO          OOOOOOO   OOOOOOOO OO          OOO    OOOOOOO
;
;**************************************************
; Huellkurven
; Lautstaerke fuer einen Kanal wird gemaess
; Huellkurve eingestellt
;**************************************************
;
Music_Play_Huellkurve_X
music_X_no_pause
       LDA music_huellkurve_low,X   ; Huellkurven Basisadresse
       STA music_pointer
       LDA music_huellkurve_high,X
       STA music_pointer+1

       lda MUSIC_POINTER           ; pruefen, ob Huellkurve 0
       ora MUSIC_POINTER+1
       beq music_X_Huellkurve_ENDE ; --> ja: nichts abspielen

       LDY Music_in_Huellkurve_Pointer,X
;?music_cmd_was_START
       LDA (music_pointer),Y

       CMP #ENDE              ; ist Huellk. zu Ende?
       BEQ music_X_Huellkurve_ENDE

;       CMP #START
;       bne ?music_X_huellkurve_not_start_again
;
;       lda #0
;       sta Music_in_Huellkurve_Pointer,x
;       tay
;       jmp ?music_cmd_was_START

;
; Huellkurve noch nicht zu ende, Counter weiterzaehlen
;
;?music_X_huellkurve_not_start_again
       STA MUSIC_AUDC
       and #$f0
       sta music_timbre,x

       iny                      ; zu naechster Note
       tya
       sta Music_in_Huellkurve_Pointer,X

       lda music_note,x
       sta music_freq
       jsr music_convertNoteZuAtariTone ; stores to music_audf,x

;
; Vibration
;
?music_vibra = music_freq

       lda ?music_is_vibra_possible,X
       beq ?music_no_vibration

       lda music_audc
       and #~00010000
       beq ?music_no_vibration   ; --> bit ist 0, keine Vibration

; wir nutzen das vollen naechste Byte fuer die Vibration

       inc Music_in_Huellkurve_Pointer,X
       lda (music_pointer),y    ; 8 bit vibration value
       bmi ?add_as_negative

       clc
       adc ?music_current_vibration_low,x
       sta ?music_current_vibration_low,x
       lda ?music_current_vibration_high,x
       adc #0
       sta ?music_current_vibration_high,x
       jmp ?music_no_vibration

?add_as_negative
       clc
       adc ?music_current_vibration_low,x
       sta ?music_current_vibration_low,x
       lda ?music_current_vibration_high,x
       adc #$FF
       sta ?music_current_vibration_high,x

?music_no_vibration
       cpx #2
       bne ?set8bit_audio

       lda ?music_audctl
       and #audctl_34_16bit
       bne ?set16bit_Audio3and4

?set8bit_audio
       sec
       lda music_audf,x
       sbc ?music_current_vibration_low,x

music_store_only
       sta music_audf,x

music_X_H2
       ldy music_x_to_audn,x  ; holt Pokey offset aus Tabelle, killt uns Akku nicht!

       sta AUDF1,y            ; Tone an POKEY geben
       stx music_save_x

       lda music_timbre,x
       lsr
       lsr
       lsr
       lsr
       lsr
       tax

       LDA MUSIC_AUDC         ; Hk-Stuetzpunkt
       AND #~00001111         ; Nur noch Lautstaerke nutzen

       ORA music_real_timbre,X
       AND #~11101111         ; Klick-Bit ausmaskieren
       STA AUDC1,Y            ; an POKEY geben

       ldx music_save_x
       RTS

music_x_to_audn
      .byte 0,2,4,6

?vibration_mal_28
 .word 0

; Hier wird hard Audio Canal 2&3 verwendet
; 16 Bit only

?set16bit_Audio3and4
; TODO: klaeren wie das mit der Vibration hier funktionieren koennte
; da wir hier mit 1.79MHz arbeiten, statt 64k also Faktor 28
       sty music_save_y
       stx music_save_x

       ldy ?music_current_vibration_low,x
       lda ?music_current_vibration_high,x
       tax
       tya
; LDA VIBRATION ; (6)
; LDX VIBRATION+1
       STX ?OP
       STA ?PRODUKT
       STX ?PRODUKT+1
       ASL A
       ROL ?OP
       ASL A
       ROL ?OP
       ASL A
       ROL ?OP
       SEC
       SBC ?PRODUKT
       TAY
       LDA ?OP
       SBC ?PRODUKT+1
       STA ?OP
       TYA
       ASL A
       ROL ?OP
       ASL A
       ROL ?OP
       STA ?vibration_mal_28 ; (44)
       LDX ?OP
       STX ?vibration_mal_28+1

       sec
       lda music_audf+2   ; low value
       sbc ?vibration_mal_28
       sta audf3          ; low in Pokey
       lda music_audf+3   ; high value
       sbc ?vibration_mal_28+1
       sta audf4          ; high in Pokey

       ldx music_save_x

       lda music_timbre,x
       lsr
       lsr
       lsr
       lsr
       lsr
       tax

       lda #0             ; immer 0
       sta AUDC3

       LDA MUSIC_AUDC      ; Hk-Stuetzpunkt
       AND #~00001111      ; Nur noch Lautstaerke nutzen

       ORA music_real_timbre,X
       AND #~11101111      ; Klick-Bit ausmaskieren
       STA AUDC4           ; an POKEY geben

       ldx music_save_x
       ldy music_save_y
       RTS

;
;                       OO
;                       OO
; OOOOOOOO   OOOOOOO  OOOOOO  OOOOOOO   OOOOOOO
; OO     OO OO     OO   OO   OO     OO OO
; OO     OO OO     OO   OO   OOOOOOOOO  OOOOOOO
; OO     OO OO     OO   OO   OO               OO
; OO     OO  OOOOOOO     OOO  OOOOOOO   OOOOOOO
;

;**************************************************
; NTSC Tabelle fuer Timbrel $A0 bei 64k
; Tabelle ist berechnet, aus einer Frequenztabelle
;**************************************************

; Hier gilt: es gibt nur diese, alle anderen AUDF Werte klingen anders
NTSC_timbre_buzzy_DISCa ; use $C0
 ; ---- Oktave 0 C0
 .byte 0,0,0,0,0,0,0,0,0,0,0,0
; .byte $ff,$f3,$e4,$d9,$cd,$c1,$b5,$ab,$a2,$99,$8e,$87
 ; ---- Oktave 1 C1
; .byte 130,123,117,108,103, 97, 93, 85, 82, 76, 72, 67
 ; ---- Oktave 2 C2
; .byte  63, 61, 57, 55, 51, 48, 45, 43, 40, 37, 36, 33
 ; ---- Oktave 3 C3
; .byte  31, 30, 28, 27, 25, 71, 22, 21, 62, 18, 53, 16

 ; ---- Oktave 1 C1
 .byte $7F ;  == 33.3  --- C-1 to G-1 are the stable, gritty timbres, ok tuning
 .byte $79 ;  == 34.9  --- These notes are not optimised, test...
 .byte $73 ;  == 36.7
 .byte $6C ;  == 39.1
 .byte $66 ;  == 41.4
 .byte $60 ;  == 43.9
 .byte $5A ;  == 46.8
 .byte $55 ;  == 49.6
 .byte $F2 ;  == 52.6  --- good with Dist A 97, (Buzzy really begins here!!!)
 .byte $E6 ;  == 55.3  --- ok with Dist C 71, great with Dist A 8F
 .byte $D7 ;  == 59.2  --- Perfect resonance to Dist C 6B, Dist A 86, 87 ok
 .byte $CB ;  == 62.7  --- Perfect resonance to Dist A FE

 ; ---- Oktave 2 C2
 .byte $BF ;  == 66.6  --- Perfect resonance to Dist A EF
 .byte $B6 ;  == 69.9  --- Best resonance to Dist A E4, E3 ok
 .byte $AA ;  == 74.8  (Dist A D5, best resonance) OR  AD  == 73.5 (Dist A D8 resonance, D7 ok)
 .byte $A1 ;  == 78.9  --- Best resonance to Dist A C9, CB ok
 .byte $98 ;  == 83.6  --- Best resonance to Dist A BE, BF ok
 .byte $8F ;  == 88.8  --- Perfect resonance to Dist A B3
 .byte $89 ;  == 92.6  --- Best resonance to Dist A AC, AB ok
 .byte $80 ;  == 99.1  --- Best resonance to Dist A A0, A1 ok
 .byte $7A ;  == 103.9 --- Best resonance to Dist A 99, 97 ok*
 .byte $71 ;  == 112.1 --- Best resonance to Dist A 8E, 8F ok*
 .byte $6B ;  == 118.4 --- Best resonance to Dist A 86, 87 ok*
 .byte $65 ;  == 125.3 --- Best resonance to Dist A 7F

 ; ---- Oktave 3 C3
 .byte $5F ;  == 133.2 --- Perfect resonance to Dist A 77
 .byte $5C ;  == 137.5 --- Best resonance to Dist A 73, 71 off*
 .byte $56 ;  == 147.0 --- Shit gets out of tune from this point lol ---
 .byte $50 ;  == 157.8 --- Perfect octave with Dist C A1
 .byte $4D ;  == 163.9
 .byte $47 ;  == 177.6 --- Perfect octave with Dist C 8F
 .byte $44 ;  == 185.3 --- Perfect octave with Dist C 89
 .byte $41 ;  == 193.7
 .byte $3E ;  == 202.9 --- offtune
 .byte $38 ;  == 224.3 --- Perfect octave with Dist C 71
 .byte $35 ;  == 236.8 --- Perfect octave with Dist C 6B
 .byte $32 ;  == 250.7 --- Perfect octave with Dist C 65

 ; ---- Oktave 4 C4
 .byte $2F ;  == 266.3 --- Perfect octave with Dist C 5F
 .byte 0,0,0,0,0,0,0,0,0,0,0
; .byte $1E ;  ?? 137.5 --- off by 1 octave, gritty table
; .byte $1C ;  ?? 146.9 --- off by 1 octave, gritty table
; .byte $29 ;  == 304.4 --- offtune
; .byte $26 ;  == 327.8 --- offtune
; .byte $23 ;  == 355.2 --- in-tune to Dist A, perfect octave with Dist C 47
; .byte $16 ;  ?? 185.3 --- off by 1 octave, gritty table
; .byte $20 ;  == 387.4 --- offtune
; .byte $31 ;  == 426.2 --- offtune, randomly mutes
; .byte $12 ;  ?? 224.3 --- off by 1 octave, gritty table
;
; .byte $1A ;  == 473.5 --- lucky in-tune note lol
; .byte $10 ;  ?? 250.7 --- off by 1 octave, gritty table
; .byte $17 ;  == 532.7 --- in-tune to Dist A, perfect octave with Dist C 2F
;


; Hier gilt: es gibt nur diese, alle anderen AUDF Werte klingen anders
NTSC_timbre_smooth_DISCb ; use $E0 to mark difference
 .byte 0,0,0,0,0,0,0,0,0,0,0,0
 ;---- Oktave 1 kleinste spielbare G#1
 .byte 0,0,0,0,0,0,0,0,245,233,218,206
 ;---- Oktave 2 C1
 .byte 197,182,173,167,155,146,137,131,122,116,110,101
 ;---- Oktave 3 C2
 .byte  98, 92, 86, 80, 77, 71, 68, 65, 62, 56, 53, 50
 ;---- Oktave 4 C3
 .byte  47,0,0,41,38,35,0,32,0,0,26,0

; NTSC Tabelle fuer Timbrel $A0 bei 64k
; Tabelle ist berechnet, aus einer Frequenztabelle
; 4 volle Oktaven
NTSC_64k_DISA
 ;---- Oktave 0
 .byte 0,0,0,0,0,0,0,0,0,0,0,0
 ;---- Oktave 1 startet mit (C1)
 .byte 0,0,0,0,0,0,0,0,0,0,0,0
 ;---- Oktave 2 C2
 .byte 0,0,0,0,0,0,0,0,0,0,0,255
 ;---- Oktave 3 C3
 .byte 243,229,216,204,192,182,171,162,152,144,136,128
 ;---- Oktave 4 C4 (enthaelt 440Hz Kammerton A)
 .byte 121,114,107,101,95,90,85,80,75,71,67,63
 ;---- Oktave 5
 .byte 60,56,53,50,47,44,42,39,37,35,33,31
 ;---- Oktave 6
 .byte 29,27,26,24,23,21,20,19,18,17,16,15
 ;---- Oktave 7
 .byte 14,13

; VORSICHT: Um ein paar Bytes zu sparen, und da die Werte sowieso nicht vorhanden sind
; Arbeiten wir hier mit einem Versatz. Spart 22bytes + 9bytes + 9bytes
@PAL_64k_DISA
 .byte 0,0,0,0,0,0,0,0,0,0
 .byte 0,0,0,0,0,0,0,0,0,0,0,0

; PAL Tabelle fuer Timbrel $A0 bei 64k
; Tabelle ist berechnet, aus einer Frequenztabelle
; 4 volle Oktaven
; ---- Oktave 0 C0
;VERSATZ! .byte 0,0,0,0,0,0,0,0,0,0,0,0
 ;---- Oktave 1 startet mit (C1)
;VERSATZ! .byte 0,0,0,0,0,0,0,0,0,0
 .byte 0,0
 ; C2,CIS2,D2,DIS2,E2,F2,FIS2,G2,GIS2,A2,AIS2,B2
 .byte 0,0,0,0,0,0,0,0,0,0,0,255
 ;---- Oktave 3 C3
 .byte 241,228,215,203,191,180,170,161,152,143,135,$7F
 ;---- Oktave 4 C4 (A4 == 440Hz Kammerton A)
 .byte 120,113,107,101,95,90,85,80,75,71,67,63
 ; C5
 .byte 60,56,53,50,47,44,42,39,37,35,33,31
 ; C6
 .byte 29,28,26,24,23,22,20,19,18,17,16,15
 ; C7
 ; ab CIS7 (13) nicht mehr wirklich gut, Frequenzen zu ungenau
 .byte 14,13,0,12,11,0,10,9,0,8,0,7
 .byte 0,0,0
; VERSATZ! .byte 0,0,0,0,0,0,0,0,0

; 7 volle Oktaven sind hier eingetragen, es ging aber noch viel mehr (9 Oktaven)
@PAL_1_79m_DISA
@PAL_1_79m_DISA_low
;       C0     C#0     D0     D#0    E0     F0     F#0    G0     G#0    A0     A#0    B0
 .byte <$0000,<$0000,<$0000,<$0000,<$0000,<$0000,<$0000,<$0000,<$0000,<$7F17,<$77EF,<$7136 ; C0
 .byte <$6AE0,<$64DC,<$5F32,<$59DC,<$54D2,<$500F,<$4B8E,<$4750,<$4350,<$3F88,<$3BF7,<$3898 ; C1
 .byte <$356A,<$326A,<$2F96,<$2CEA,<$2A64,<$2803,<$25C4,<$23A5,<$21A4,<$1FC0,<$1DF8,<$1C49 ; C2
 .byte <$1AB2,<$1932,<$17C8,<$1672,<$152F,<$13FE,<$12DE,<$11CF,<$10CF,<$0FDD,<$0EF8,<$0E21 ; C3
 .byte <$0D55,<$0C96,<$0BE0,<$0B35,<$0A94,<$09FB,<$096C,<$08E4,<$0864,<$07EB,<$0779,<$070D ; C4
 .byte <$06A7,<$0647,<$05ED,<$0597,<$0546,<$04FA,<$04B2,<$046E,<$042E,<$03F2,<$03B9,<$0383 ; C5
 .byte <$034E,<$0320,<$02F3,<$02C8,<$02A0,<$027A,<$0256,<$0234,<$0214,<$01F5,<$01D9,<$01BE ; C6
 .byte <$01A5,<$018D,<$0176,<$0161,<$014c,<$0139,<$0127,<$0116,<$0106,<$00f7,<$00e9,<$00db ; C7
 .byte <$00cf,<$00c3,<$00b7
 ;VERSATZ! .byte ,<$0000,<$0000,<$0000,<$0000,<$0000,<$0000,<$0000,<$0000,<$0000 ; C8
@PAL_1_79m_DISA_high
 .byte >$0000,>$0000,>$0000,>$0000,>$0000,>$0000,>$0000,>$0000,>$0000,>$7F17,>$77EF,>$7136
 .byte >$6AE0,>$64DC,>$5F32,>$59DC,>$54D2,>$500F,>$4B8E,>$4750,>$4350,>$3F88,>$3BF7,>$3898
 .byte >$356A,>$326A,>$2F96,>$2CEA,>$2A64,>$2803,>$25C4,>$23A5,>$21A4,>$1FC0,>$1DF8,>$1C49
 .byte >$1AB2,>$1932,>$17C8,>$1672,>$152F,>$13FE,>$12DE,>$11CF,>$10CF,>$0FDD,>$0EF8,>$0E21
 .byte >$0D55,>$0C96,>$0BE0,>$0B35,>$0A94,>$09FB,>$096C,>$08E4,>$0864,>$07EB,>$0779,>$070D
 .byte >$06A7,>$0647,>$05ED,>$0597,>$0546,>$04FA,>$04B2,>$046E,>$042E,>$03F2,>$03B9,>$0383
 .byte >$034E,>$0320,>$02F3,>$02C8,>$02A0,>$027A,>$0256,>$0234,>$0214,>$01F5,>$01D9,>$01BE
 .byte >$01A5,>$018D,>$0176,>$0161,>$014c,>$0139,>$0127,>$0116,>$0106,>$00f7,>$00e9,>$00db
 .byte >$00cf,>$00c3,>$00b7,>$0000,>$0000,>$0000,>$0000,>$0000,>$0000,>$0000,>$0000,>$0000
;
;**************************************************
;Tabellen der Basisadressen fuer Noten-
;und Huellkurven Tabellen.
;**************************************************

;pacmen
;    .word length
;    .word huellkurven
;    .word Shapes
;    .word takt
;
;; will insert by player
;Length
;       .byte 0,0,0,0,0,0,0,0
;
;huellkurven
;       .WORD 0,HUELL1,huell2
;
;huell1
; .byte BASS2|10,0,BASS2|10,0,BASS2|9,0,BASS2|9,0,BASS2|8,0,BASS2|8,0,BASS2|7,0,BASS2|7,0,BASS2|6,0,BASS2|6,0,BASS2|5,0,BASS2|5,0,BASS2|4,0,BASS2|4,0,BASS2|3,0,BASS2|3,0,BASS2|2,0,BASS2|2,0,BASS2|1,0,BASS2|1,0,BASS2|0,0,ENDE
;;huell1
;;       .BYTE BASS2|10,0,BASS2|10,0,BASS2|10,0,BASS2|9,0,BASS2|9,0,BASS2|9,0,BASS2|9,0,BASS2|9,0,BASS2|9,0,BASS2|9,0
;;       .byte BASS2|8 ,0,BASS2|8 ,0,BASS2|8 ,0,BASS2|8,0,BASS2|8,0,BASS2|7,0,BASS2|7,0,BASS2|7,0,BASS2|7,0,BASS2|7,0
;;       .byte BASS2|6 ,0,BASS2|6 ,0,BASS2|6 ,0,BASS2|6,0,BASS2|6,0,BASS2|5,0,BASS2|5,0,BASS2|5,0,BASS2|5,0,BASS2|5,0
;;       .byte BASS2|4 ,0,BASS2|4 ,0,BASS2|4 ,0,BASS2|4,0,BASS2|4,0,BASS2|3,0,BASS2|3,0,BASS2|3,0,BASS2|3,0,BASS2|3,0
;;       .byte BASS2|2 ,0,BASS2|2 ,0,BASS2|2 ,0,BASS2|2,0,BASS2|2,0,BASS2|1,0,BASS2|1,0,BASS2|1,0,BASS2|1,0,BASS2|1,0,BASS2|0,0,ENDE ; 2. Teile Saege
;
;huell2
; .byte $ac,$ab,$aa,$a9,$a8,$a7,$a6,$a5,$a4,$a3,$a2,$a1,$a0,$a0,ENDE
;; ------------------------------------------------------------------------------
;shapes
;    .word 0,shape_bass1,shape_test
;
;SHAPE_BASS1
; .byte HUELL,1
; .byte ST_B1,Q,ST_B2,Q,ST_B1,Q,ST_B2,Q,ENDE
;
;SHAPE_TEST
; .byte huell,2
; .byte a4,q,ende
;
;takt
;    .byte SPEED,0,0,0
;    .byte 2,0,0,0
;    .byte ENDE,0,0,0
;
;
;; IDEE: Um zu sagen wie die 4 Kanaele verwendet werden sollen:
;; .byte USAGE,4,8,0     ; nur 8bit
;; .byte USAGE,8,8,16    ; 2 x 8bit 1x 16bit
;; .byte USAGE,2,16,0   ; nur 16bit
