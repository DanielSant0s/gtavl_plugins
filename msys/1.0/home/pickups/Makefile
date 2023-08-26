# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2001-2004, ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
#
# $Id: Makefile.sample 1150 2005-06-12 11:42:08Z pixel $

EE_ERL = pickups.erl

EE_STDLIB = plugin_ps2/src/math.o \
		   plugin_ps2/src/stdio.o \
		   plugin_ps2/src/stdlib.o \
		   plugin_ps2/src/string.o \
		   plugin_ps2/src/injector.o

EE_SALIB = plugin_sa/src/common.o \
		   plugin_sa/src/CVector.o \
		   plugin_sa/src/CVehicle.o \
		   plugin_sa/src/CTexture.o \
		   plugin_sa/src/CPad.o \
		   plugin_sa/src/CTxdStore.o \
		   plugin_sa/src/CRadar.o \
		   plugin_sa/src/CRGBA.o \
		   plugin_sa/src/CFont.o \
		   plugin_sa/src/CCam.o \
		   plugin_sa/src/rwcore.o

EE_OBJS = $(EE_STDLIB) $(EE_SALIB) src/pickups.o

EE_INCS += -Iplugin_sa/include/
EE_INCS += -Iplugin_ps2/include/

EE_CFLAGS = -fno-builtin-memcmp -fno-builtin-memcpy -fno-builtin-memset
EE_CFLAGS += -fno-builtin-printf -fno-builtin-strcat -fno-builtin-strchr
EE_CFLAGS += -fno-builtin-strcmp -fno-builtin-strcpy -fno-builtin-strlen
EE_CFLAGS += -fno-builtin-strncpy -fno-builtin-strrchr -fno-builtin-strncmp
EE_CFLAGS += -fno-builtin-cosf -fno-builtin-sinf -fno-builtin-sprintf

all: $(EE_ERL)

clean:
	rm -rf *.erl src/*.o
	rm -rf plugin_sa/src/*.o
	rm -rf plugin_ps2/src/*.o

include Makefile.pref
include Makefile.eeglobal

