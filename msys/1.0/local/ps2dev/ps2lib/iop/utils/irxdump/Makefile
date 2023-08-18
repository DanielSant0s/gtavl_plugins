#
# IRXDump Makefile
#
.SUFFIXES: .s .c .o .irx

top = ../../..
iop = $(top)/iop

CC = gcc
LD = gcc
CFLAGS = 
DEFS = 

INCLUDES = -I../elf2irx -I$(top)/common/loadelf -I$(top)/common/include 
COMPILE = $(CC) $(INCLUDES) $(CFLAGS) $(DEFS)
LINK = $(CC) -o $@

all: irxdump

.c.o:
	$(COMPILE) -c $<
.s.o:
	$(COMPILE) -c $<

loadelf.o: $(top)/common/loadelf/loadelf.c
	$(COMPILE) -c $(top)/common/loadelf/loadelf.c

irxdump_OBJ = irxdump.o loadelf.o
irxdump: $(irxdump_OBJ)
	$(LINK) $(irxdump_OBJ)

clean: 
	rm -f irxdump $(irxdump_OBJ)
