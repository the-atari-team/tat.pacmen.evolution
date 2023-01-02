; -*- text -*-
;RANDOM-Library
;fuer den WiNiFe-Compiler
;cdw by 'The Atari Team' 2020
;
   .LOCAL

;
; OO     OO  OOOOOO OOOOOOO
; OO     OO    OO   OO    OO
; OO     OO    OO   OO    OO
; OO     OO    OO   OO    OO
; OO  O  OO    OO   OOOOOOO
; OO  O  OO    OO   OO
;  OOO OOO   OOOOOO OO
;

; TODO: Test ueber die Normalverteilung des Random erstellen

; per default the good old Atari has a real random value (53770)
; @type: function
; @return a 16 bit random Value
@random65536
@random16bit
      LDY RANDOM
      LDX RANDOM
      RTS

@random32767
@random15bit
@randomPositiv
      LDY RANDOM
      LDA RANDOM
      AND #~01111111
      TAX
      RTS

@random256
@random8bit
      LDY RANDOM
      LDX #0
      RTS

@random512
@random9bit
      LDY RANDOM
      LDA RANDOM
      AND #~00000001
      TAX
      RTS

@random128
@random7bit
      LDA RANDOM
      AND #~01111111 ; 127
      TAY
      LDX #0
      RTS

; need other Values for random, use @randomMod(i) for RANDOM mod i


;?x=@reg
;?y=@reg+1
;?modulus=@reg+2
;
;?pseudomodulo
;  SEC
;  LDA ?x
;  SBC ?modulus
;  ADC ?y
;  BCS ?end
;  ADC ?modulus
;?end
;  rts

; @random(x) return a random value not greater than x
; multiply x (16bit) with random (16bit) in 32bit  and return the upper 16 bit
; expensive but good
@random
@random_i
      LDY #1
      LDA (@HEAP_PTR),Y ; low not-gt value
      STA @OP
      INY
      LDA (@HEAP_PTR),Y ; high not-gt value

      beq ?inner_8bit_random

      STA @OP+1

;      winc @op

; ((@OP + 256 * @OP)+1) * random
      LDA RANDOM
      STA @OP+2
      LDA RANDOM
      STA @OP+3
      jsr ?mult16

      LDY @PRODUKT+2
      LDX @PRODUKT+3
      RTS

; 8 bit random value, loops as long as given value is greater or equal than random
; @fastrand8(value)
; @return a byte between [0, value - 1]
@fastrand8
@fastrand8_i
  ldy #1
  lda (@HEAP_PTR),y
  beq ?fail

  sta @op
?inner_8bit_random

?loop
  LDY RANDOM   ; Atari RANDOM is a 17bit counter and 9bit counter?
  cpy @op
  bcs ?loop    ; --> if RANDOM greater or equal @op

?fail
;  TAY
  LDX #0
  RTS


;// Pick a random number
;function rand(min, max) {
; min = Math.ceil(min);
; max = Math.floor(max);
; let rand = Math.random();
; return Math.floor(rand * (max - min + 1)) + min;
;}
?min=@REG
?max=@REG+2
@rand
@rand_ii
      LDY #1
      LDA (@HEAP_PTR),Y
      STA ?min
      INY
      LDA (@HEAP_PTR),Y
      STA ?min+1

      INY
      LDA (@HEAP_PTR),Y
      STA ?max
      INY
      LDA (@HEAP_PTR),Y
      STA ?max+1

      ; SEC
      CLC
      LDA ?max
      SBC ?min
      STA @OP
      LDA ?max+1
      SBC ?min+1
      STA @OP+1   ; max - min + 1 // +1 set CLC

; (@OP + 256 * @OP+1) * random
      LDA RANDOM
      STA @OP+2
      LDA RANDOM
      STA @OP+3
      jsr ?mult16

      LDA @PRODUKT+2
      CLC
      ADC ?min
      TAY
      LDA @PRODUKT+3
      ADC ?min+1
      TAX
      RTS

;16-bit multiply with 32-bit product
;took from 6502.org

?multiplier	= @OP
?multiplicand	= @OP+2
?product    = @PRODUKT

?mult16
      lda	#$00
      sta	?product+2	; clear upper bits of product
      sta	?product+3
      ldx	#$10    ; set binary count to 16
shift_r
      lsr	?multiplier+1	; divide multiplier by 2
      ror	?multiplier
      bcc	rotate_r

      lda	?product+2	; get upper half of product and add multiplicand
      clc
      adc	?multiplicand
      sta	?product+2
      lda	?product+3
      adc	?multiplicand+1
rotate_r
      ror    	      ; rotate partial product
      sta	?product+3
      ror	?product+2
      ror	?product+1
      ror	?product
      dex
      bne	shift_r
      rts

; just for tests
@randomECHO
@randomECHO_I
  ldy #1              ; 2
  lda (@heap_ptr),y   ; 6
  tay                 ; 2
  ldx #0              ; 2
  rts                 ; 6 => sum 18 + 6 jsr

; random(n) with (32768 mod n) fuehrt leider zu uebelsten Interferenzen
; deshalb ist es kein guter Random!
;@random2
;@random2_i
;      LDY #1
;      LDA (@HEAP_PTR),Y
;      STA @OP
;      INY
;      LDA (@HEAP_PTR),Y
;      STA @OP+1
;
; (@OP + 256 * @OP+1) * random
;      LDY RANDOM
;      LDA RANDOM           ; high byte must not negativ
;      AND #~01111111
;      TAX
;      jmp @IMOD
;
; Interferencen extrem bei:
; 39 45 74 78 90 91 92 109 116 117 135 136 137 138 145 148 156 164 166 178
; 180 181 182 184 188 190 195 198 203 205 209 217 218 219 222 225 230 232
; 233 234 235 237 239 241 244 249 267 268 269 270 271 272 273 274 275 276
; 277 278 279 283 286 290 292 293 294 295 296 297 299 300 301 307 309 310
; 312 313 315 317 318
