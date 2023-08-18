EE_BIN = siosh_dont_run_this.elf
EE_LIB = libsiosh.a
EE_OUTPUT=siosh.elf
EE_OBJS = exceptions.o sio_shell.o R5900Disasm.o ymodem.o iopmon.o
EE_LIBS = libsiosh.a -ldebug
all: $(EE_OUTPUT)

$(EE_OUTPUT) : $(EE_BIN)
		ps2-packer -s stub/n2e-kmode-1d00-stub $(EE_BIN) $(EE_OUTPUT)
		rm -f $(EE_BIN)

$(EE_BIN) : main.o $(EE_LIB)
		$(EE_CC) -mno-crt0 -nostartfiles -Tlinkfile $(EE_LDFLAGS) -o $(EE_BIN) main.o $(EE_LIBS)

clean:
	rm -f $(EE_BIN) $(EE_OUTPUT) *.o *.a *.bin

include Makefile.pref
include Makefile.eeglobal

