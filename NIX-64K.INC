; -*- text -*-
;
; Pacmen nix 64K
;
; Wir implementieren ein paar Stubs
;

; THIS FILE WILL INCLUDE BY OTHER FILE
;

 .local

; Damit koennen wir 64k Code verwenden, ohne wirklich 64k zu haben

@INIT_SIMPLE_64K
@ACTIVATE_RAM
@ACTIVATE_ROM
  RTS

