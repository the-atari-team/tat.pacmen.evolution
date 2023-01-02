; NOTEN Definition fuer HEXEN Musik
; --------------------------------------------------------------------------------
; Eine Hexen-Musik besteht aus einer Liste mit 4 Zeigern.

;    .word Length        ; Zeigt auf 8 Byte Liste [0] ist volle Note,[1] halbe...
;    .word huellkurven
;    .word Shapes
;    .word takt          ; wo faengt die Liste der Takte an


; EQUALES der CLEAN-NOTEN ------------------------------------------

;
;                       OO
;                       OO
; OOOOOOOO   OOOOOOO  OOOOOO  OOOOOOO   OOOOOOO
; OO     OO OO     OO   OO   OO     OO OO
; OO     OO OO     OO   OO   OOOOOOOOO  OOOOOOO
; OO     OO OO     OO   OO   OO               OO
; OO     OO  OOOOOOO     OOO  OOOOOOO   OOOOOOO
;

; see: https://de.wikipedia.org/wiki/Frequenzen_der_gleichstufigen_Stimmung
; von Note 1 zu 2 ist immer Faktor 1.0594630943593 (12. Wurzel aus 2)
; CLEAN (1010)     64kHz        15kHz
C0     = 0       ;              16.35Hz Oktave 0
CIS0   = 1
D0     = 2
DIS0   = 3
E0     = 4       ;              20.6Hz
F0     = 5
FIS0   = 6
G0     = 7
GIS0   = 8
A0     = 9       ;              27.5Hz
AIS0   = 10
B0     = 11      ;              30.87Hz

C1     = 12      ; -            32.7Hz Oktave 1
CIS1   = 13
D1     = 14
DIS1   = 15
E1     = 16
F1     = 17
FIS1   = 18
G1     = 19
GIS1   = 20
A1     = 21      ; -            55Hz
AIS1   = 22
B1     = 23

C2     = 24      ;              65.41Hz Oktave 2
CIS2   = 25
D2     = 26
DIS2   = 27
E2     = 28
F2     = 29
FIS2   = 30
G2     = 31
GIS2   = 32
A2     = 33      ;              110Hz
AIS2   = 34
B2     = 35      ; tiefste Note 64k 123.47Hz(255)

C3     = 36      ; 130.8Hz(241) Oktave 3
CIS3   = 37
D3     = 38
DIS3   = 39
E3     = 40
F3     = 41
FIS3   = 42
G3     = 43
GIS3   = 44
A3     = 45      ; 220Hz
AIS3   = 46
B3     = 47      ;                 246.9Hz

C4     = 48      ; Oktave 4 261Hz
CIS4   = 49
D4     = 50
DIS4   = 51
E4     = 52
F4     = 53
FIS4   = 54
G4     = 55
GIS4   = 56
A4     = 57      ; 440Hz(70)       Kammerton A
AIS4   = 58
B4     = 59      ;                 493.88Hz

C5     = 60      ; Oktave 5 523Hz
CIS5   = 61
D5     = 62
DIS5   = 63
E5     = 64
F5     = 65
FIS5   = 66
G5     = 67
GIS5   = 68
A5     = 69      ; 880Hz
AIS5   = 70
B5     = 71

C6     = 72      ; Oktave 6 1049Hz
CIS6   = 73
D6     = 74
DIS6   = 75
E6     = 76
F6     = 77
FIS6   = 78
G6     = 79
GIS6   = 80
A6     = 81      ; 1760Hz
AIS6   = 82
B6     = 83

C7     = 84      ; Oktave 7 2093Hz
CIS7   = 85      ; hoechste Note bei 64kHz
D7     = 86
DIS7   = 87
E7     = 88
F7     = 89
FIS7   = 90
G7     = 91
GIS7   = 92
A7     = 93
AIS7   = 94
B7     = 95

C8     = 96      ; Oktave 8 (64k/2/(1+14)) => Freq 2133Hz

; EQUALES der BASS-NOTEN ------------------------------------------

; BASS smooth
; BASS2 ($D0 & ~11010000)
; In HEXEN set $E0 as timbrel
;
; Playable tone starts with GIS1


; buzzy timbre
; BASS1 ($C0)
; In HEXEN set $C0 as timbrel

;BT_C1   = 0   ; Oktave 1
;BT_CIS1 = 1
;BT_D1   = 2
;BT_DIS1 = 3
;BT_E1   = 4
;BT_F1   = 5
;BT_FIS1 = 6
;BT_G1   = 7
;BT_GIS1 = 8
;BT_A1   = 9
;BT_AIS1 = 10
;BT_B1   = 11
;
;BT_C2   = 12   ; Oktave 2
;BT_CIS2 = 13
;BT_D2   = 14
;BT_DIS2 = 15
;BT_E2   = 16
;BT_F2   = 17
;BT_FIS2 = 18
;BT_G2   = 19
;BT_GIS2 = 20
;BT_A2   = 21
;BT_AIS2 = 22
;BT_B2   = 23
;
;BT_C3   = 24   ; Oktave 3
;BT_CIS3 = 25
;BT_D3   = 26
;BT_DIS3 = 27
;BT_E3   = 28
;BT_F3   = 29   ; ???
;BT_FIS3 = 30
;BT_G3   = 31
;BT_GIS3 = 32   ; ???
;BT_A3   = 33
;BT_AIS3 = 34   ; ???
;BT_B3   = 35

; EQUATES fuer Kommandos

; VORSICHT!: Da viele Kommandos auch innerhalb von Shapes verwendet werden
;            muessen wir darauf achten nicht mit den Noten zu kollidieren
;

ENDE  =    $7E        ; 126 Code fuer Stimmende
START =    $7D        ; 125 Code fuer Stimm-Neubeginn
JUMP  =    252

; TIMBREL = 251
; T       = TIMBREL
HUELL      = 250
AUDIO_INIT = 249

PAUSE =    248        ; fuer Verlaengerung der Note
P     =   PAUSE       ; Kuerzel f. Pause

; Disortion (Verzerrung) wird jetzt in der Huellkurve untergebracht
BASS2 =   $E0 ; SMOOTH ($C0 + $20)  ~1110  4-bit poly
BASS1 =   $C0 ; BUZZY je nach Noten ~1100  4-bit poly
CLEAN =   $A0 ;                     ~1x10
DRUMS =   $80 ;                     ~1000 17-bit poly
EFFECT1 = $40 ;                     ~0100 5-bit poly dann 4-bit poly
EFFECT2 = $20 ;                     ~0x10 5-bit poly
EFFECT3 = $00 ;                     ~0000 5-bit poly dann 17-bit poly

; EQUATES fuer Tonelaengen ---------------------------------------

; Normale Dauer in 1/50s (16tel Note ist 6/50s lang auf PAL Maschinen)
; Normale Dauer in 1/60s (16tel Note ist 7/60s lang auf NTSC Maschinen)

; Laenge einer Note in 1/50s
; Voll(F), halb(H), viertel(Q), achtel(E), 16tel(S), 32tel

F      =   0  ;            ; Volle Note
H      =   1  ; DAUER*8    ; Halbe Note
Q      =   2  ; DAUER*4    ; Viertel Note (Quarter)
E      =   3  ; DAUER*2    ; Achtel Note (Eight)
S      =   4  ; DAUER      ; Sechzehntel Note
V      =   5  ; 255 1/50s  ; Haengt jetzt an Laenge der Huellkurve


; Huellkurven Tabellen ----------------------------------------
; Es werden Stuetzpunkte der Huellkurven
; als Lautstaerkewerte eingetragen
; jede Tabelle beginnt mit HUELL<n>
; und endet mit 'ENDE'
;
; Atack, release, decline...


; Shape Tabellen -----------------------------------------------
; Ein Shape besagt, welche Note(n) wie lange gespielt werden soll.
; Ein Shape ist immer eine volle Note lang. (~2s in Normalspeed)
; Es gibt aber ein paar Einstellungen, gerade fuer die reinen Noten
; diese sind als EQUATES abgelegt und gucken dann in einer Tabelle nach dem AUDF-Wert
; Note, laenge
;
; Weitere Marker:
; TIMBREL,Value für Atari Rauschgeneratoren 4bit, 5bit, 17bit, ... der Wert landet im
; AUDC Register und enthaelt zusaetzlich die Lautstaerke. Das Klick-Bit wird maskiert
; ein TIMBREL muss gesetzt werden, sonst wissen wir nicht, was gespielt werden soll.
; eine Huellkurve muss gesetzt werden, nur so wird die Lautstaerke gesetzt.
;
; einige TIMBREL-Werte sorgen dafuer, das Noten mit Hilfe einer Tabelle uebersetzt werden.
; $A0 oder $E0 ist reiner Ton, es wird eine Note C0 - C7 erwartet und aus einer Liste in ein AUDFn uebersetzt
; $C0 ist BASS1 BUZZY  (4bit poly) es wird eine Note BT_C0 - BT_B2 erwartet
; $D0 ist BASS2 SMOOTH (4bit poly) es wird eine Note ST_GIS0 - ST_B2 erwartet

; alle anderen TIMBREL siehe Profibuch Seite: 221(Abbuc Version 2018)
; $00    ; 5bit dann 17bit poly
; $20    ; 5bit poly
; $40    ; 5bit dann 4bit
; $80    ; 17bit poly ist für DRUMS
; hier werden die Werte direkt an AUDFn weitergeleitet. Hier ist zu beachten, das
; die Marker nicht funktionieren (kleine Einschränkung)
;
; HUELL,Value  ; der Value sagt, welche Huellkurve genutzt werden soll
; ENDE         ; Der Shape ist hier zu Ende
;
; Ein Shape kann bis zu 256 Bytes lang sein, meist reichen aber weniger als 32 Bytes,
; Er muss aber wenigsten 7 Bytes haben. TIMBREL,wert,HUELL,wert, Note, laenge, ENDE
;
; Ein TIMBREL oder HUELL sorgt NICHT fuer eine Verzoegerungen.
; Es werden solange Werte aus dem Shape gelesen,
; bis AUDF und AUFC in einer 1/50s gesetzt sind.


; Takte-List ----------------------------------------------
SPEED_0= 0 ; very fast ~48f (<1s) per takt
SPEED_1= 1
SPEED_2= 2
SPEED_3= 3
SPEED_4= 4
SPEED_5= 5
SPEED_6= 6
SPEED_7= 7 ; very slow ~160f (>3s) per takt

AUDCTL_9BIT_POLY=~10000000
AUDCTL_12_1_79M= ~01000000
AUDCTL_34_1_79M= ~00100000
AUDCTL_12_16BIT= ~00010000
AUDCTL_34_16BIT= ~00001000
AUDCTL_15K=      ~00000001
AUDCTL_0=         0

; Es gibt ein paar Marker:
; AUDIO_INIT, SPEED_0-7,AUDCTL,0 ; 0=very fast, 7 very slow
; JUMP, ziel_low, ziel_high,0    ; JUMP-Sprung zum Ziel
; ENDE,0,0,0                     ; Musik hört auf
; START,0,0,0                    ; wie JUMP,0,0,0
; sonst sind es immer indirekte Zeiger auf die Shapes

