
EE_BIN = c-wrapper.elf
EE_OBJS = main.o Gif.o

all : $(EE_BIN)

# ps2sdk isn't ANSI C compatible, so we need this seperate rule
main.o : main.c
	$(EE_CC) $(EE_INCS) -O2 -c $< -o $@

include $(LIBITO)/Makefile.prefab


