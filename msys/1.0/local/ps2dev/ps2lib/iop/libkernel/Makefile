IOP_LIB = ../lib/libkernel.a
IOP_OBJS = iop_cdvdman.o iop_heaplib.o iop_intrman.o iop_ioman.o iop_libsd.o iop_loadcore.o iop_modload.o iop_sifcmd.o iop_sifman.o iop_stdio.o iop_sysclib.o iop_sysmem.o iop_thbase.o iop_thevent.o iop_thsemap.o iop_usbd.o iop_vblank.o

all: $(IOP_LIB)

clean:
	rm -f *.elf *.o *.a

include ../../Makefile.pref
include ../../Makefile.iopglobal
