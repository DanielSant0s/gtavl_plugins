# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2001-2004, ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
#
# $Id: Makefile 1522 2009-01-22 09:48:56Z radad $

EE_ERL = base.erl

EE_INC_DIR = include/

#EE_INCS = -I$(PS2SDKSRC)/ee/libc/include -I$(PS2SDKSRC)/ee/erl-loader/src

EE_OBJS = src/erl.o src/hashtab.o src/lookupa.o src/recycle.o src/erl-loader.o src/wrappers.o

# Since we are using non-builtin symbols
EE_CFLAGS = -fno-builtin-memcmp -fno-builtin-memcpy -fno-builtin-memset
EE_CFLAGS += -fno-builtin-printf -fno-builtin-strcat -fno-builtin-strchr
EE_CFLAGS += -fno-builtin-strcmp -fno-builtin-strcpy -fno-builtin-strlen
EE_CFLAGS += -fno-builtin-strncpy -fno-builtin-strrchr -fno-builtin-strncmp

all: $(EE_ERL)

clean:
	rm -f -r $(EE_ERL)

include Makefile.pref
include Makefile.eeglobal
