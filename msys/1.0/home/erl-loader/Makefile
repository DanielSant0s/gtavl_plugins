# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2001-2004, ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
#
# $Id: Makefile 1522 2009-01-22 09:48:56Z radad $


EE_OBJS_DIR = obj/
EE_BIN_DIR = bin/
EE_SRC_DIR = src/
EE_INC_DIR = include/

#EE_INCS = -I$(PS2SDKSRC)/ee/libc/include -I$(PS2SDKSRC)/ee/erl-loader/src

EE_OBJS = erl.o hashtab.o lookupa.o recycle.o erl-loader.o wrappers.o
EE_OBJS := $(EE_OBJS:%=$(EE_OBJS_DIR)%)

EE_LDFLAGS =  -mno-crt0 -Tlinkfile

# Since we are using non-builtin symbols
EE_CFLAGS = -fno-builtin-memcmp -fno-builtin-memcpy -fno-builtin-memset
EE_CFLAGS += -fno-builtin-printf -fno-builtin-strcat -fno-builtin-strchr
EE_CFLAGS += -fno-builtin-strcmp -fno-builtin-strcpy -fno-builtin-strlen
EE_CFLAGS += -fno-builtin-strncpy -fno-builtin-strrchr -fno-builtin-strncmp

LOADER_BIN = relocator.elf

all: $(EE_OBJS_DIR) $(EE_BIN_DIR) $(LOADER_BIN)
	sh -c "./export_plugin_api.sh"

$(LOADER_BIN) : $(EE_OBJS)
	$(EE_CC) $(EE_CFLAGS) -o $(LOADER_BIN) $(EE_OBJS) $(EE_LDFLAGS) $(EE_LIBS) 

clean:
	rm -f -r $(EE_OBJS_DIR) $(EE_BIN_DIR)

include $(PS2SDKSRC)/Defs.make
include $(PS2SDKSRC)/ee/Rules.make
include $(PS2SDKSRC)/ee/Rules.release
