# Makefile to build and start The Pacmen-evolution
CC=./compiler/bin/wnfc
ASM=atasm
PACKER=./compiler/bin/xl-packer
ALTIRRA=wine /home/lars/.wine/drive_c/atarixl.software/AtariXL_400/Altirra-410b5.exe

ATARI800_OPTIONS='-kbdjoy0'

FIRMWARE=../firmware
COMPILER=.

WNFC_OPTIONS='-O' 2 '-smallHeapPtr'

.PHONY: copy_turbobasic_files prepare_atari clean all prepare info

# You need to give the "PROGRAM name" name out of wnf file here, add .65o
GAME_OBJ_FILES=\
PACFONT.INC\
PACFONT2.INC\
TITLFONT.INC\
PACINTRO.INC\
SHUFFLE.INC \
PACMEN.65o

TESTS=

INCLUDES=GHOSTS.INC \
 PACMDATA.INC \
 PACMOVE.INC \
 GHSTMOVE.INC \
 COOKIE.INC \
 FADE.INC \
 EXTRA.INC \
 LEVEL.INC \
 AUDIOPLY.INC \
 SHOWSCR.INC

ADDITIONALS=\
autoload-test-runner.lst \
test-runner-d.lst \
no-color-switch.com \
Turbo-Basic_XL_2.0.com \
.os.txt \
AUTORUN.TUR

all: $(INCLUDES) $(TESTS) $(EDITOR_BUILD_FILES) $(GAME_OBJ_FILES) atari_game

info:
	@echo
	@echo "OOOOO                                      "
	@echo "OO  OO                                     "
	@echo "OO  OO  OOOOO  OOOO  OOO OO   OOOO  OOOOO  "
	@echo "OO  OO OO  OO OO  OO OOOOOOO OO  OO OO  OO "
	@echo "OOOOO  OO  OO OO     OO O OO OOOOOO OO  OO "
	@echo "OO     OO  OO OO  OO OO O OO OO     OO  OO "
	@echo "OO      OOOOO  OOOO  OO O OO  OOOO  OO  OO "
	@echo


clean:: info
	rm -f .wnffiles.txt
	rm -f $(ADDITIONALS) $(GAME_OBJ_FILES) $(INCLUDES)
	rm -f *.COM *.ASM *.65o .insert.txt *.lab *.log
	rm -f PACMEN.ASM.inc PACMEN.lst
	rm -f huffman-decode.lst

clean::
	rm -f start-game.atr start-test.atr
	rm -f editor.atr

TESTPACM.65o: test-pacmen.wnf pacmen-struct.INC HIGHSCORE.INC draw-ghosts.INC draw-pacmen.INC $(GAME_OBJ_FILES) $(INCLUDES) GAME-DLI.INC
	$(CC) $< $(WNFC_OPTIONS) -I $(COMPILER)
	$(ASM) $(@:.65o=.ASM) -l$(@:.65o=.lab) >/dev/null
	cp $@ $(@:.65o=.COM)

PACMEN.65o: game-pacmen.wnf pacmen-struct.INC HIGHSCORE.INC draw-ghosts.INC draw-pacmen.INC huffman-decode.INC $(INCLUDES) GAME-DLI.INC compress
	$(CC) $< $(WNFC_OPTIONS) -showvariableusage -I $(COMPILER)
	$(ASM) -ha -s $(@:.65o=.ASM) -g$(@:.65o=.lst) -l$(@:.65o=.lab) >$(@:.65o=.log)
	cp $@ $(@:.65o=.COM)

FLOYD.65o: generate-floydwarshall-matrix.wnf
	$(CC) $< $(WNFC_OPTIONS) -I $(COMPILER)
	$(ASM) $(@:.65o=.ASM) -l$(@:.65o=.lab) >$(@:.65o=.log)
	cp $@ $(@:.65o=.COM)

PACFONT.INC: pacmen-font.wnf
	$(CC) $< -noheader $(WNFC_OPTIONS) -I $(COMPILER)

TITLFONT.INC: pacmen-title-font.wnf
	$(CC) $< $(WNFC_OPTIONS) -I $(COMPILER)

PACFONT2.INC: pacmen-font-2.wnf
	$(CC) $< $(WNFC_OPTIONS) -I $(COMPILER)

GHOSTS.INC: ghosts-data.wnf
	$(CC) $< $(WNFC_OPTIONS) -I $(COMPILER)

FADE.INC: fade.wnf
	$(CC) $< $(WNFC_OPTIONS) -I $(COMPILER)

PACMDATA.INC: pacmen-data.wnf
	$(CC) $< $(WNFC_OPTIONS) -I $(COMPILER)

AUDIOPLY.INC: audio-play.wnf
	$(CC) $< $(WNFC_OPTIONS) -I $(COMPILER)

PACMOVE.INC: move-pacmen.wnf
	$(CC) $< $(WNFC_OPTIONS) -showvariableusage -I $(COMPILER)

GHSTMOVE.INC: move-ghosts.wnf
	$(CC) $< $(WNFC_OPTIONS) -showvariableusage -I $(COMPILER)

PACINTRO.INC: pacmen-title-intro.wnf
	$(CC) $< $(WNFC_OPTIONS) -I $(COMPILER)

COOKIE.INC: cookie.wnf
	$(CC) $< $(WNFC_OPTIONS) -I $(COMPILER)

SHUFFLE.INC: shuffle.wnf
	$(CC) $< $(WNFC_OPTIONS) -I $(COMPILER)

EXTRA.INC: extra.wnf
	$(CC) $< $(WNFC_OPTIONS) -I $(COMPILER)

LEVEL.INC: level.wnf
	$(CC) $< $(WNFC_OPTIONS) -I $(COMPILER)

SHOWSCR.INC: show-score.wnf
	$(CC) $< $(WNFC_OPTIONS) -I $(COMPILER)

#
# Pacmen-Editor
#

prepare_game: prepare
# Dann das Spiel selbst
	touch .wnffiles.txt
	echo "PACMEN.COM -> AUTORUN.SYS" >.wnffiles.txt

prepare_last:
	cat .wnffiles.txt >>.insert.txt
	echo "stop" >>.wnffiles.txt

game_disk: prepare_game prepare_last
	xldir $(COMPILER)/dos-sd.atr insert .insert.txt start-game.atr

test_disk: copy_turbobasic_files prepare_turbobasic_test prepare_last
	xldir $(COMPILER)/starter17.atr insert .insert.txt start-test.atr

atari_game: game_disk
	atari800 \
    -hreadwrite \
    -H2 /tmp/atari \
    -xlxe_rom $(FIRMWARE)/ATARIXL.ROM \
    -xl -xl-rev 2 \
    -pal -showspeed -nobasic \
    -windowed -win-width 682 -win-height 482 \
    -vsync -video-accel \
    ${ATARI800_OPTIONS} \
   start-game.atr

atari_test: test_disk
	atari800 \
    -hreadwrite \
    -H2 /tmp/atari \
    -xlxe_rom $(FIRMWARE)/ATARIXL.ROM \
    -xl -xl-rev 2 \
    -pal -showspeed -nobasic \
    -windowed -win-width 682 -win-height 482 \
    -vsync -video-accel \
    ${ATARI800_OPTIONS} \
   start-test.atr

# Start Altirra with debug and stop at start address
# /portable searchs for an Altirra.ini
debug: $(INCLUDES) $(GAME_OBJ_FILES) game_disk
	$(ALTIRRA) \
	/portable \
	/debug \
	/debugcmd: ".sourcemode on" \
	/debugcmd: ".loadsym PACMEN.lst" \
	/debugcmd: ".loadsym PACMEN.lab" \
	/debugcmd: "bp 2000" \
	/disk "start-game.atr"


prepare:
	rm -f .wnffiles.txt
	rm -f .insert.txt
	echo ".os.txt -> OS.TXT" >>.insert.txt

# game_tests: prepare $(GAME_OBJ_FILES) game_disk

n: $(INCLUDES) $(GAME_OBJ_FILES)

start: n atari_game

test: $(INCLUDES) $(TESTS) atari_test

PLAYFIELDS=\
 PLAYS0.DAT \
 PLAYS1.DAT \
 PLAYS2.DAT \
 PLAYS3.DAT \
 PLAYS4.DAT \
 PLAYS5.DAT \
 PLAYS6.DAT \
 PLAYS7.DAT \
 PLAYS8.DAT \
 PLAYS9.DAT \
 PLAYSA.DAT \
 PLAYSB.DAT \
 PLAYSC.DAT \
 PLAYSD.DAT \
 PLAYSE.DAT \
 PLAYSF.DAT \
 PLAYSG.DAT \
 PLAYSH.DAT \
 PLAYSI.DAT \
 PLAYSJ.DAT \
 PLAYSK.DAT \
 PLAYSL.DAT \
 PLAYSM.DAT \
 PLAYSN.DAT \
 PLAYSO.DAT \
 PLAYSP.DAT \
 PLAYSQ.DAT \
 PLAYSR.DAT \
 PLAYSS.DAT \
 PLAYST.DAT \
 PLAYSU.DAT \
 PLAYSV.DAT \
 PLAYSW.DAT \
 PLAYSX.DAT \
 PLAYSY.DAT \
 PLAYSZ.DAT

.PHONY: $(PLAYFIELDS)

clean::
	rm -f COMPRESSED-PLAYFIELDS.INC

# compress all playfields
COMPRESSED-PLAYFIELDS.INC: $(PLAYFIELDS)
	$(PACKER) --data -of $@ $(PLAYFIELDS)

compress: COMPRESSED-PLAYFIELDS.INC

help:
	@echo "Makefile for the Atari 8bit game Pacmen"
	@echo "usage:"
	@echo "make             - to create the whole game disk and start it"
	@echo "make n           - just build, do not start"
	@echo "make clean       - remove all build files"
	@echo "make start       - start game in atari800 emulator"
	@echo "make help        - show this list"
