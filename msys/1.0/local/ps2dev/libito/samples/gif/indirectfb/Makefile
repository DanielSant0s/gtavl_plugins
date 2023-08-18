
EE_BIN = indirectfb.elf
EE_OBJS = main.o clut.o

CLEAN_FILES = *.iif

all : $(EE_BIN)


%.s : %.iif
	$(BIN2S) $< $@ fire_clut

clut.iif : fire_clut.bmp
	$(IIT) -c -p CLUT8-RGBA32 $< $@

include $(LIBITO)/Makefile.prefab
