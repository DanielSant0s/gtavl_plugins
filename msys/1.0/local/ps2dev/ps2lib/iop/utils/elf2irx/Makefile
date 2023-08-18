#
# ELF2IRX Makefile
#
.SUFFIXES: .s .c .o .irx

top = ../../..
iop = $(top)/iop

CC = gcc
LD = gcc
CFLAGS = 
DEFS = 

INCLUDES = -I$(top)/common/include -I$(top)/common/loadelf
COMPILE = $(CC) $(INCLUDES) $(CFLAGS) $(DEFS)
LINK = $(CC) -o $@

all: elf2irx

.c.o:
	$(COMPILE) -c $<
.s.o:
	$(COMPILE) -c $<

loadelf.o: $(top)/common/loadelf/loadelf.c
	$(COMPILE) -c $(top)/common/loadelf/loadelf.c

elf2irx_OBJ = elf2irx.o loadelf.o
elf2irx: $(elf2irx_OBJ)
	$(LINK) $(elf2irx_OBJ)

clean: 
	rm -f elf2irx $(elf2irx_OBJ)
