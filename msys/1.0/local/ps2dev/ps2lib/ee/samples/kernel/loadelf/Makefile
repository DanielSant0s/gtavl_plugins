TOPDIR = $(PS2LIB)

EE_BIN  = loadelf.elf
EE_OBJS = loadelf.o

EE_LDFLAGS += -Wl,-Ttext -Wl,0x200000

all: $(EE_BIN)

clean:
	-rm -f *.elf *.o

include $(TOPDIR)/Makefile.pref
include $(TOPDIR)/Makefile.eeglobal
